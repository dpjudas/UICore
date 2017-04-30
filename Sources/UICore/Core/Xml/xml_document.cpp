/*
**  UICore SDK
**  Copyright (c) 1997-2015 The UICore Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries UICore may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#include "UICore/precomp.h"
#include "UICore/Core/Xml/xml_node.h"
#include "UICore/Core/Xml/xml_document.h"
#include "UICore/Core/Xml/xml_tokenizer.h"
#include "UICore/Core/Xml/xml_writer.h"
#include "UICore/Core/Xml/xml_token.h"
#include "xml_node_impl.h"
#include "xml_document_impl.h"
#include "xml_tree_node.h"
#include <stack>

namespace uicore
{
	std::shared_ptr<XmlDocument> XmlDocument::create()
	{
		auto doc = std::make_shared<XmlDocumentImpl>();
		doc->node_index = doc->allocate_tree_node(XmlNodeType::document);
		return doc;
	}

	std::shared_ptr<XmlDocument> XmlDocument::load(const std::shared_ptr<IODevice> &input, bool eat_whitespace)
	{
		auto doc = XmlDocument::create();

		auto tokenizer = XmlTokenizer::create(input);
		tokenizer->set_eat_whitespace(eat_whitespace);

		std::vector<std::shared_ptr<XmlNode>> node_stack;
		node_stack.push_back(doc);

		XmlToken cur_token;
		tokenizer->next(&cur_token);
		while (cur_token.type != XmlTokenType::null)
		{
			switch (cur_token.type)
			{
			case XmlTokenType::text:
				node_stack.back()->append_child(doc->create_text(cur_token.value));
				break;

			case XmlTokenType::cdata:
				node_stack.back()->append_child(doc->create_cdata(cur_token.value));
				break;

			case XmlTokenType::element:
				if (cur_token.variant != XmlTokenVariant::end)
				{
					XmlString namespace_uri = XmlDocumentImpl::find_namespace_uri(cur_token.name, cur_token, node_stack.back());
					auto element = doc->create_element(namespace_uri, cur_token.name);
					node_stack.back()->append_child(element);

					int size = (int)cur_token.attributes.size();
					for (int i = 0; i < size; i++)
					{
						XmlToken::Attribute &attribute = cur_token.attributes[i];
						XmlString attribute_namespace_uri = XmlDocumentImpl::find_namespace_uri(attribute.first, cur_token, node_stack.back());
						element->set_attribute(attribute_namespace_uri, attribute.first, attribute.second);
					}

					if (cur_token.variant == XmlTokenVariant::begin)
						node_stack.push_back(element);
				}
				else
				{
					node_stack.pop_back();
					if (node_stack.empty()) throw Exception("Malformed XML tree!");
				}
				break;

			case XmlTokenType::null:
				break;

			case XmlTokenType::comment:
				node_stack.back()->append_child(doc->create_comment(cur_token.value));
				break;

			case XmlTokenType::doctype:
				break;

			case XmlTokenType::processing_instruction:
				//node_stack.back()->append_child(doc->create_processing_instruction(cur_token.name, cur_token.value));
				break;
			}

			tokenizer->next(&cur_token);
		}

		return doc;
	}

	/////////////////////////////////////////////////////////////////////////

	XmlDocumentImpl::XmlDocumentImpl()
	{
	}

	XmlDocumentImpl::~XmlDocumentImpl()
	{
		std::vector<XmlNodeImpl *>::size_type pos, size;

		size = nodes.size();
		for (pos = 0; pos < size; pos++)
			delete nodes[pos];

		while (!free_dom_nodes.empty())
		{
			delete free_dom_nodes.back();
			free_dom_nodes.pop_back();
		}
	}

	std::shared_ptr<XmlNode> XmlDocumentImpl::document_element() const
	{
		for (auto cur = first_child(); cur; cur = cur->next_sibling())
		{
			if (cur->is_element())
				return cur;
		}
		return nullptr;
	}

	std::shared_ptr<XmlNode> XmlDocumentImpl::first_child() const
	{
		const XmlTreeNode *tree_node = get_tree_node();
		if (tree_node->first_child != cl_null_node_index)
		{
			return const_cast<XmlDocumentImpl*>(this)->allocate_dom_node(tree_node->first_child);
		}
		else
		{
			return nullptr;
		}
	}

	std::shared_ptr<XmlNode> XmlDocumentImpl::last_child() const
	{
		const XmlTreeNode *tree_node = get_tree_node();
		if (tree_node->last_child != cl_null_node_index)
		{
			return const_cast<XmlDocumentImpl*>(this)->allocate_dom_node(tree_node->last_child);
		}
		else
		{
			return nullptr;
		}
	}

	std::shared_ptr<XmlNode> XmlDocumentImpl::insert_before(const std::shared_ptr<XmlNode> &new_child, const std::shared_ptr<XmlNode> &ref_child)
	{
		if (!ref_child)
		{
			append_child(new_child);
			return new_child;
		}
		else if (new_child && ref_child)
		{
			if (new_child->parent())
				new_child->parent()->remove_child(new_child);

			auto new_child_impl = static_cast<XmlNodeImpl*>(new_child.get());
			auto ref_child_impl = static_cast<XmlNodeImpl*>(ref_child.get());
			XmlTreeNode *tree_node = get_tree_node();
			XmlTreeNode *new_tree_node = new_child_impl->get_tree_node();
			XmlTreeNode *ref_tree_node = ref_child_impl->get_tree_node();

			new_tree_node->previous_sibling = ref_tree_node->previous_sibling;
			new_tree_node->next_sibling = ref_child_impl->node_index;
			ref_tree_node->previous_sibling = new_child_impl->node_index;
			if (new_tree_node->previous_sibling != cl_null_node_index)
				new_tree_node->get_previous_sibling(this)->next_sibling = new_child_impl->node_index;
			if (tree_node->first_child == ref_child_impl->node_index)
				tree_node->first_child = new_child_impl->node_index;
			new_tree_node->parent = node_index;

			return new_child;
		}
		else
		{
			return nullptr;
		}
	}

	std::shared_ptr<XmlNode> XmlDocumentImpl::replace_child(const std::shared_ptr<XmlNode> &new_child, const std::shared_ptr<XmlNode> &old_child)
	{
		if (new_child && old_child)
		{
			if (new_child->parent())
				new_child->parent()->remove_child(new_child);

			auto new_child_impl = static_cast<XmlNodeImpl*>(new_child.get());
			auto old_child_impl = static_cast<XmlNodeImpl*>(old_child.get());

			XmlTreeNode *tree_node = get_tree_node();
			XmlTreeNode *new_tree_node = new_child_impl->get_tree_node();
			XmlTreeNode *old_tree_node = old_child_impl->get_tree_node();

			new_tree_node->previous_sibling = old_tree_node->previous_sibling;
			new_tree_node->next_sibling = old_tree_node->next_sibling;
			new_tree_node->parent = node_index;
			if (tree_node->first_child == old_child_impl->node_index)
				tree_node->first_child = new_child_impl->node_index;
			if (tree_node->last_child == old_child_impl->node_index)
				tree_node->last_child = new_child_impl->node_index;
			old_tree_node->previous_sibling = cl_null_node_index;
			old_tree_node->next_sibling = cl_null_node_index;
			old_tree_node->parent = cl_null_node_index;

			return new_child;
		}
		else
		{
			return nullptr;
		}
	}

	std::shared_ptr<XmlNode> XmlDocumentImpl::remove_child(const std::shared_ptr<XmlNode> &old_child)
	{
		if (old_child)
		{
			auto old_child_impl = static_cast<XmlNodeImpl*>(old_child.get());
			XmlTreeNode *tree_node = get_tree_node();
			XmlTreeNode *old_tree_node = old_child_impl->get_tree_node();
			unsigned int prev_index = old_tree_node->previous_sibling;
			unsigned int next_index = old_tree_node->next_sibling;
			XmlTreeNode *prev = old_tree_node->get_previous_sibling(this);
			XmlTreeNode *next = old_tree_node->get_next_sibling(this);
			if (next)
				next->previous_sibling = prev_index;
			if (prev)
				prev->next_sibling = next_index;
			if (tree_node->first_child == old_child_impl->node_index)
				tree_node->first_child = next_index;
			if (tree_node->last_child == old_child_impl->node_index)
				tree_node->last_child = prev_index;
			old_tree_node->previous_sibling = cl_null_node_index;
			old_tree_node->next_sibling = cl_null_node_index;
			old_tree_node->parent = cl_null_node_index;
			return old_child;
		}
		else
		{
			return nullptr;
		}
	}

	std::shared_ptr<XmlNode> XmlDocumentImpl::append_child(const std::shared_ptr<XmlNode> &new_child)
	{
		if (new_child)
		{
			if (new_child->parent())
				new_child->parent()->remove_child(new_child);

			auto new_child_impl = static_cast<XmlNodeImpl*>(new_child.get());
			XmlTreeNode *tree_node = get_tree_node();
			XmlTreeNode *new_tree_node = new_child_impl->get_tree_node();
			if (tree_node->last_child != cl_null_node_index)
			{
				XmlTreeNode *last_tree_node = tree_node->get_last_child(this);
				last_tree_node->next_sibling = new_child_impl->node_index;
				new_tree_node->previous_sibling = tree_node->last_child;
				tree_node->last_child = new_child_impl->node_index;
			}
			else
			{
				tree_node->first_child = new_child_impl->node_index;
				tree_node->last_child = new_child_impl->node_index;
			}
			new_tree_node->parent = node_index;
			return new_child;
		}
		else
		{
			return nullptr;
		}
	}

	std::shared_ptr<XmlNode> XmlDocumentImpl::clone(bool deep) const
	{
		auto mutable_this = const_cast<XmlDocumentImpl*>(this);
		return mutable_this->import_node(mutable_this->shared_from_this(), deep);
	}

	std::shared_ptr<XmlNode> XmlDocumentImpl::named_item(const XmlString &name) const
	{
		auto node = document_element();
		if (node && node->name() == name)
			return node;
		else
			return nullptr;
	}

	std::shared_ptr<XmlNode> XmlDocumentImpl::named_item(const XmlString &namespace_uri, const XmlString &local_name) const
	{
		auto node = document_element();
		if (node && node->namespace_uri() == namespace_uri && node->local_name() == local_name)
			return node;
		else
			return nullptr;
	}

	std::shared_ptr<XmlNode> XmlDocumentImpl::create_element(const XmlString &tag_name)
	{
		auto element = allocate_dom_node(allocate_tree_node(XmlNodeType::element));
		auto tree_node = element->get_tree_node();
		tree_node->node_name = tag_name;
		return element;
	}

	std::shared_ptr<XmlNode> XmlDocumentImpl::create_element(const XmlString &namespace_uri, const XmlString &qualified_name)
	{
		auto element = allocate_dom_node(allocate_tree_node(XmlNodeType::element));
		auto tree_node = element->get_tree_node();
		tree_node->namespace_uri = namespace_uri;
		tree_node->node_name = qualified_name;
		return element;
	}

	std::shared_ptr<XmlNode> XmlDocumentImpl::create_fragment()
	{
		return allocate_dom_node(allocate_tree_node(XmlNodeType::document_fragment));
	}

	std::shared_ptr<XmlNode> XmlDocumentImpl::create_text(const XmlString &data)
	{
		auto text = allocate_dom_node(allocate_tree_node(XmlNodeType::text));
		auto tree_node = text->get_tree_node();
		tree_node->node_value = data;
		return text;
	}

	std::shared_ptr<XmlNode> XmlDocumentImpl::create_comment(const XmlString &data)
	{
		auto comment = allocate_dom_node(allocate_tree_node(XmlNodeType::comment));
		auto tree_node = comment->get_tree_node();
		tree_node->node_value = data;
		return comment;
	}

	std::shared_ptr<XmlNode> XmlDocumentImpl::create_cdata(const XmlString &data)
	{
		auto cdata = allocate_dom_node(allocate_tree_node(XmlNodeType::cdata));
		auto tree_node = cdata->get_tree_node();
		tree_node->node_value = data;
		return cdata;
	}

	std::shared_ptr<XmlNode> XmlDocumentImpl::create_attribute(const XmlString &name)
	{
		auto attribute = allocate_dom_node(allocate_tree_node(XmlNodeType::attribute));
		auto tree_node = attribute->get_tree_node();
		tree_node->node_name = name;
		return attribute;
	}

	std::shared_ptr<XmlNode> XmlDocumentImpl::create_attribute(const XmlString &namespace_uri, const XmlString &qualified_name)
	{
		auto attribute = allocate_dom_node(allocate_tree_node(XmlNodeType::attribute));
		auto tree_node = attribute->get_tree_node();
		tree_node->namespace_uri = namespace_uri;
		tree_node->node_name = qualified_name;
		return attribute;
	}

	std::shared_ptr<XmlNode> XmlDocumentImpl::import_node(const std::shared_ptr<XmlNode> &node, bool deep)
	{
		std::shared_ptr<XmlNode> imported_node;
		switch (node->type())
		{
		case XmlNodeType::element:
			imported_node = create_element(node->namespace_uri(), node->name());
			break;
		case XmlNodeType::attribute:
			imported_node = create_attribute(node->namespace_uri(), node->name());
			imported_node->set_text(node->text());
			break;
		case XmlNodeType::text:
			imported_node = create_text(node->text());
			break;
		case XmlNodeType::cdata:
			imported_node = create_cdata(node->text());
			break;
		case XmlNodeType::comment:
			imported_node = create_comment(node->text());
			break;
		case XmlNodeType::document:
			imported_node = create_fragment();
			break;
		case XmlNodeType::document_fragment:
			imported_node = create_fragment();
			break;
		}

		for (auto attr = node->first_attribute(); attr; attr = attr->next_sibling())
		{
			imported_node->add_attribute(import_node(attr, deep));
		}

		if (deep)
		{
			for (auto cur = node->first_child(); cur; cur = cur->next_sibling())
			{
				imported_node->append_child(import_node(cur, true));
			}
		}

		return imported_node;
	}

	void XmlDocumentImpl::save(const std::shared_ptr<IODevice> &output, bool insert_whitespace) const
	{
		auto writer = XmlWriter::create(output);
		writer->set_insert_whitespace(insert_whitespace);

		std::vector<std::shared_ptr<XmlNode>> node_stack;
		auto cur_node = first_child();
		while (cur_node)
		{
			// Create opening node:
			XmlToken opening_node;
			switch (cur_node->type())
			{
			default: break;
			case XmlNodeType::element: opening_node.type = XmlTokenType::element; break;
			case XmlNodeType::text: opening_node.type = XmlTokenType::text; break;
			case XmlNodeType::cdata: opening_node.type = XmlTokenType::cdata; break;
			case XmlNodeType::comment: opening_node.type = XmlTokenType::comment; break;
			}
			opening_node.variant = cur_node->has_child_nodes() ? XmlTokenVariant::begin : XmlTokenVariant::single;
			opening_node.name = cur_node->name();
			if (cur_node->is_element())
			{
				for (auto attr = cur_node->first_attribute(); attr; attr = attr->next_sibling())
				{
					opening_node.attributes.push_back(XmlToken::Attribute(attr->name(), attr->text()));
				}
			}
			else
			{
				opening_node.value = cur_node->text();
			}
			writer->write(opening_node);

			// Create any possible child nodes:
			if (cur_node->has_child_nodes())
			{
				node_stack.push_back(cur_node);
				cur_node = cur_node->first_child();
				continue;
			}

			// Create closing nodes until we reach next opening node in tree:
			while (true)
			{
				if (cur_node->has_child_nodes())
				{
					XmlToken closing_node;
					closing_node.type = XmlTokenType::element;
					closing_node.name = cur_node->name();
					closing_node.variant = XmlTokenVariant::end;
					writer->write(closing_node);
				}

				cur_node = cur_node->next_sibling();
				if (cur_node)
					break;
				if (node_stack.empty())
					break;

				cur_node = node_stack.back();
				node_stack.pop_back();
			}
		}
	}

	XmlString XmlDocumentImpl::find_namespace_uri(const XmlString &qualified_name, const XmlToken &search_token, const std::shared_ptr<XmlNode> &search_node)
	{
		static XmlString xmlns_prefix("xmlns:");
		XmlString name = qualified_name;
		XmlString prefix;
		XmlString::size_type pos = name.find(':');
		if (pos != XmlString::npos)
			prefix = name.substr(0, pos);

		int size = (int)search_token.attributes.size();
		for (int i = 0; i < size; i++)
		{
			if (prefix.empty())
			{
				if (search_token.attributes[i].first == "xmlns")
					return search_token.attributes[i].second;
			}
			else
			{
				if (search_token.attributes[i].first.substr(0, 6) == xmlns_prefix &&
					search_token.attributes[i].first.substr(6) == prefix)
					return search_token.attributes[i].second;
			}
		}

		return search_node->find_namespace_uri(qualified_name);
	}

	unsigned int XmlDocumentImpl::allocate_tree_node(XmlNodeType type)
	{
		if (free_nodes.empty())
		{
			auto node = new (&node_allocator) XmlTreeNode();
			node->node_type = type;
			nodes.push_back(node);
			return nodes.size() - 1;
		}
		else
		{
			unsigned index = free_nodes.back();
			nodes[index]->reset();
			nodes[index]->node_type = type;
			free_nodes.pop_back();
			return index;
		}
	}

	void XmlDocumentImpl::free_tree_node(unsigned int node_index)
	{
		free_nodes.push_back(node_index);
	}

	std::shared_ptr<XmlNodeImpl> XmlDocumentImpl::allocate_dom_node(unsigned int node_index)
	{
		auto public_node = nodes[node_index]->public_node.lock();
		if (public_node)
			return public_node;

		XmlNodeImpl *node = nullptr;
		if (free_dom_nodes.empty())
		{
			node = new XmlNodeImpl;
			node->_owner_document = std::static_pointer_cast<XmlDocumentImpl>(shared_from_this());
			node->node_index = node_index;
		}
		else
		{
			node = free_dom_nodes.back();
			node->_owner_document = std::static_pointer_cast<XmlDocumentImpl>(shared_from_this());
			node->node_index = node_index;
			free_dom_nodes.pop_back();
		}

		public_node = std::shared_ptr<XmlNodeImpl>(node, [this](XmlNodeImpl *ptr) { free_dom_node(ptr); });
		nodes[node_index]->public_node = public_node;
		return public_node;
	}

	void XmlDocumentImpl::free_dom_node(XmlNodeImpl *node)
	{
		auto owner = node->_owner_document;
		node->_owner_document = nullptr;
		free_dom_nodes.push_back(node);
	}
}
