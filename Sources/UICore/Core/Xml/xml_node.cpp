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
#include "xml_node_impl.h"
#include "xml_document_impl.h"
#include "xml_tree_node.h"

namespace uicore
{
	XmlString XmlNodeImpl::name() const
	{
		const XmlTreeNode *tree_node = get_tree_node();
		switch (tree_node->node_type)
		{
		case XmlNodeType::cdata:
			return "#cdata-section";
		case XmlNodeType::comment:
			return "#comment";
		case XmlNodeType::document:
			return "#document";
		case XmlNodeType::document_fragment:
			return "#document-fragment";
		case XmlNodeType::text:
			return "#text";
		case XmlNodeType::attribute:
		case XmlNodeType::element:
		default:
			return tree_node->get_node_name();
		}
	}

	XmlString XmlNodeImpl::text() const
	{
		const XmlTreeNode *tree_node = get_tree_node();
		if (tree_node->node_type == XmlNodeType::element)
		{
			std::string str;
			XmlNodePtr cur = first_child();
			while (cur)
			{
				if (cur->is_text() || cur->is_cdata_section() || cur->is_element())
					str.append(cur->text());
				cur = cur->next_sibling();
			}
			return str;
		}
		else
		{
			return tree_node->get_node_value();
		}
	}

	XmlString XmlNodeImpl::namespace_uri() const
	{
		return get_tree_node()->get_namespace_uri();
	}

	XmlString XmlNodeImpl::prefix() const
	{
		XmlString node_name = get_tree_node()->get_node_name();
		XmlString::size_type pos = node_name.find(':');
		if (pos != XmlString::npos)
			return node_name.substr(0, pos);
		else
			return XmlString();
	}

	void XmlNodeImpl::set_prefix(const XmlString &prefix)
	{
		auto doc_impl = weak_owner_document.lock();
		XmlString node_name = get_tree_node()->get_node_name();
		XmlString::size_type pos = node_name.find(':');
		if (pos == XmlString::npos)
			get_tree_node()->set_node_name(doc_impl.get(), prefix + ':' + node_name);
		else
			get_tree_node()->set_node_name(doc_impl.get(), prefix + node_name.substr(pos));
	}

	XmlString XmlNodeImpl::local_name() const
	{
		XmlString node_name = get_tree_node()->get_node_name();
		XmlString::size_type pos = node_name.find(':');
		if (pos != XmlString::npos)
			return node_name.substr(pos + 1);
		else
			return node_name;
	}

	void XmlNodeImpl::set_text(const XmlString &value)
	{
		if (is_element())
		{
			bool complex_children = first_child() != last_child() || (first_child() && !first_child()->is_text());
			if (complex_children)
			{
				while (last_child())
					remove_child(last_child());
			}

			if (first_child())
			{
				first_child()->set_text(value);
			}
			else
			{
				append_child(owner_document()->create_text(value));
			}
		}
		else
		{
			auto doc_impl = weak_owner_document.lock();
			auto tree_node = get_tree_node();
			tree_node->set_node_value(doc_impl.get(), value);
		}
	}

	XmlNodeType XmlNodeImpl::type() const
	{
		return get_tree_node()->node_type;
	}

	XmlNodePtr XmlNodeImpl::parent() const
	{
		const XmlTreeNode *tree_node = get_tree_node();
		if (tree_node->parent != cl_null_node_index)
		{
			auto doc_impl = weak_owner_document.lock();
			if (tree_node->parent != doc_impl->node_index)
				return doc_impl->allocate_dom_node(tree_node->parent);
			else
				return doc_impl;
		}
		else
		{
			return nullptr;
		}
	}

	XmlNodePtr XmlNodeImpl::first_child() const
	{
		const XmlTreeNode *tree_node = get_tree_node();
		if (tree_node->first_child != cl_null_node_index)
		{
			auto doc_impl = weak_owner_document.lock();
			return doc_impl->allocate_dom_node(tree_node->first_child);
		}
		else
		{
			return nullptr;
		}
	}

	XmlNodePtr XmlNodeImpl::last_child() const
	{
		const XmlTreeNode *tree_node = get_tree_node();
		if (tree_node->last_child != cl_null_node_index)
		{
			auto doc_impl = weak_owner_document.lock();
			return doc_impl->allocate_dom_node(tree_node->last_child);
		}
		else
		{
			return nullptr;
		}
	}

	XmlNodePtr XmlNodeImpl::previous_sibling() const
	{
		const XmlTreeNode *tree_node = get_tree_node();
		if (tree_node->previous_sibling != cl_null_node_index)
		{
			auto doc_impl = weak_owner_document.lock();
			return doc_impl->allocate_dom_node(tree_node->previous_sibling);
		}
		else
		{
			return nullptr;
		}
	}

	XmlNodePtr XmlNodeImpl::next_sibling() const
	{
		const XmlTreeNode *tree_node = get_tree_node();
		if (tree_node->next_sibling != cl_null_node_index)
		{
			auto doc_impl = weak_owner_document.lock();
			return doc_impl->allocate_dom_node(tree_node->next_sibling);
		}
		else
		{
			return nullptr;
		}
	}

	XmlDocumentPtr XmlNodeImpl::owner_document() const
	{
		return weak_owner_document.lock();
	}

	bool XmlNodeImpl::has_attributes() const
	{
		return get_tree_node()->first_attribute != cl_null_node_index;
	}

	XmlNodePtr XmlNodeImpl::insert_before(const XmlNodePtr &new_child, const XmlNodePtr &ref_child)
	{
		if (!ref_child)
		{
			append_child(new_child);
			return new_child;
		}
		else if (new_child && ref_child)
		{
			auto doc_impl = weak_owner_document.lock();
			auto new_child_impl = static_cast<XmlNodeImpl*>(new_child.get());
			auto ref_child_impl = static_cast<XmlNodeImpl*>(ref_child.get());
			XmlTreeNode *tree_node = get_tree_node();
			XmlTreeNode *new_tree_node = new_child_impl->get_tree_node();
			XmlTreeNode *ref_tree_node = ref_child_impl->get_tree_node();

			new_tree_node->previous_sibling = ref_tree_node->previous_sibling;
			new_tree_node->next_sibling = ref_child_impl->node_index;
			ref_tree_node->previous_sibling = new_child_impl->node_index;
			if (new_tree_node->previous_sibling != cl_null_node_index)
				new_tree_node->get_previous_sibling(doc_impl.get())->next_sibling = new_child_impl->node_index;
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

	XmlNodePtr XmlNodeImpl::replace_child(const XmlNodePtr &new_child, const XmlNodePtr &old_child)
	{
		if (new_child && old_child)
		{
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

	XmlNodePtr XmlNodeImpl::remove_child(const XmlNodePtr &old_child)
	{
		if (old_child)
		{
			auto doc_impl = weak_owner_document.lock();
			auto old_child_impl = static_cast<XmlNodeImpl*>(old_child.get());
			XmlTreeNode *tree_node = get_tree_node();
			XmlTreeNode *old_tree_node = old_child_impl->get_tree_node();
			unsigned int prev_index = old_tree_node->previous_sibling;
			unsigned int next_index = old_tree_node->next_sibling;
			XmlTreeNode *prev = old_tree_node->get_previous_sibling(doc_impl.get());
			XmlTreeNode *next = old_tree_node->get_next_sibling(doc_impl.get());
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

	XmlNodePtr XmlNodeImpl::append_child(const XmlNodePtr &new_child)
	{
		if (new_child)
		{
			auto doc_impl = weak_owner_document.lock();
			auto new_child_impl = static_cast<XmlNodeImpl*>(new_child.get());
			XmlTreeNode *tree_node = get_tree_node();
			XmlTreeNode *new_tree_node = new_child_impl->get_tree_node();
			if (tree_node->last_child != cl_null_node_index)
			{
				XmlTreeNode *last_tree_node = tree_node->get_last_child(doc_impl.get());
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

	XmlNodePtr XmlNodeImpl::remove_attribute(const XmlNodePtr &old_child)
	{
		if (old_child)
		{
			auto doc_impl = weak_owner_document.lock();
			auto old_child_impl = static_cast<XmlNodeImpl*>(old_child.get());
			XmlTreeNode *tree_node = get_tree_node();
			XmlTreeNode *old_tree_node = old_child_impl->get_tree_node();
			unsigned int prev_index = old_tree_node->previous_sibling;
			unsigned int next_index = old_tree_node->next_sibling;
			XmlTreeNode *prev = old_tree_node->get_previous_sibling(doc_impl.get());
			XmlTreeNode *next = old_tree_node->get_next_sibling(doc_impl.get());
			if (next)
				next->previous_sibling = prev_index;
			if (prev)
				prev->next_sibling = next_index;
			if (tree_node->first_attribute == old_child_impl->node_index)
				tree_node->first_attribute = next_index;
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

	XmlNodePtr XmlNodeImpl::add_attribute(const XmlNodePtr &new_child)
	{
		if (new_child)
		{
			auto doc_impl = weak_owner_document.lock();
			auto new_child_impl = static_cast<XmlNodeImpl*>(new_child.get());
			XmlTreeNode *tree_node = get_tree_node();
			XmlTreeNode *new_tree_node = new_child_impl->get_tree_node();
			if (tree_node->first_attribute != cl_null_node_index)
			{
				XmlTreeNode *first_tree_node = tree_node->get_first_attribute(doc_impl.get());
				first_tree_node->previous_sibling = new_child_impl->node_index;
				new_tree_node->next_sibling = tree_node->first_attribute;
				tree_node->first_attribute = new_child_impl->node_index;
			}
			else
			{
				tree_node->first_attribute = new_child_impl->node_index;
			}
			new_tree_node->parent = node_index;
			return new_child;
		}
		else
		{
			return nullptr;
		}
	}

	XmlNodePtr XmlNodeImpl::clone(bool deep) const
	{
		return owner_document()->import_node(const_cast<XmlNodeImpl*>(this)->shared_from_this(), deep);
	}

	XmlNodePtr XmlNodeImpl::named_item(const XmlString &name) const
	{
		auto node = first_child();
		while (node)
		{
			if (node->name() == name) return node;
			node = node->next_sibling();
		}
		return nullptr;
	}

	XmlNodePtr XmlNodeImpl::named_item(const XmlString &namespace_uri, const XmlString &local_name) const
	{
		auto node = first_child();
		while (node)
		{
			if (node->namespace_uri() == namespace_uri && node->local_name() == local_name)
				return node;
			node = node->next_sibling();
		}
		return nullptr;
	}

	bool XmlNodeImpl::has_attribute(const XmlString &name) const
	{
		auto doc_impl = weak_owner_document.lock();
		const XmlTreeNode *tree_node = get_tree_node();
		unsigned int cur_index = tree_node->first_attribute;
		const XmlTreeNode *cur_attribute = tree_node->get_first_attribute(doc_impl.get());
		while (cur_attribute)
		{
			if (cur_attribute->get_node_name() == name)
				return true;

			cur_index = cur_attribute->next_sibling;
			cur_attribute = cur_attribute->get_next_sibling(doc_impl.get());
		}
		return false;
	}

	bool XmlNodeImpl::has_attribute(const XmlString &namespace_uri, const XmlString &local_name) const
	{
		for (auto attr = first_attribute(); attr; attr = attr->next_sibling())
		{
			if (attr->local_name() == local_name && attr->namespace_uri() == namespace_uri)
				return true;
		}
		return false;
	}

	XmlString XmlNodeImpl::attribute(const XmlString &name, const XmlString &default_value) const
	{
		auto doc_impl = weak_owner_document.lock();
		const XmlTreeNode *tree_node = get_tree_node();
		unsigned int cur_index = tree_node->first_attribute;
		const XmlTreeNode *cur_attribute = tree_node->get_first_attribute(doc_impl.get());
		while (cur_attribute)
		{
			if (cur_attribute->get_node_name() == name)
				return cur_attribute->get_node_value();

			cur_index = cur_attribute->next_sibling;
			cur_attribute = cur_attribute->get_next_sibling(doc_impl.get());
		}
		return default_value;
	}

	XmlString XmlNodeImpl::attribute_ns(const XmlString &namespace_uri, const XmlString &local_name, const XmlString &default_value) const
	{
		for (auto attr = first_attribute(); attr; attr = attr->next_sibling())
		{
			if (attr->local_name() == local_name && attr->namespace_uri() == namespace_uri)
				return attr->text();
		}
		return default_value;
	}

	void XmlNodeImpl::set_attribute(const XmlString &name, const XmlString &value)
	{
		for (auto attr = first_attribute(); attr; attr = attr->next_sibling())
		{
			if (attr->name() == name)
			{
				attr->set_text(value);
				return;
			}
		}

		auto attr = owner_document()->create_attribute(name);
		attr->set_text(value);
		add_attribute(attr);
	}

	void XmlNodeImpl::set_attribute(const XmlString &namespace_uri, const XmlString &qualified_name, const XmlString &value)
	{
		for (auto attr = first_attribute(); attr; attr = attr->next_sibling())
		{
			if (attr->name() == qualified_name && attr->namespace_uri() == namespace_uri)
			{
				attr->set_text(value);
				return;
			}
		}

		auto attr = owner_document()->create_attribute(namespace_uri, qualified_name);
		attr->set_text(value);
		add_attribute(attr);
	}

	void XmlNodeImpl::remove_attribute(const XmlString &name)
	{
		for (auto attr = first_attribute(); attr; attr = attr->next_sibling())
		{
			if (attr->name() == name)
			{
				remove_attribute(attr);
				return;
			}
		}
	}

	void XmlNodeImpl::remove_attribute(const XmlString &namespace_uri, const XmlString &local_name)
	{
		for (auto attr = first_attribute(); attr; attr = attr->next_sibling())
		{
			if (attr->local_name() == local_name && attr->namespace_uri() == namespace_uri)
			{
				remove_attribute(attr);
				return;
			}
		}
	}

	XmlNodePtr XmlNodeImpl::attribute_node(const XmlString &name) const
	{
		for (auto attr = first_attribute(); attr; attr = attr->next_sibling())
		{
			if (attr->name() == name)
				return attr;
		}
		return nullptr;
	}

	XmlNodePtr XmlNodeImpl::attribute_node(const XmlString &namespace_uri, const XmlString &local_name) const
	{
		for (auto attr = first_attribute(); attr; attr = attr->next_sibling())
		{
			if (attr->local_name() == local_name && attr->namespace_uri() == namespace_uri)
				return attr;
		}
		return nullptr;
	}

	XmlString XmlNodeImpl::find_namespace_uri(const XmlString &qualified_name) const
	{
		static XmlString xmlns_prefix("xmlns:");
		static XmlString xmlns_xml("xml");
		static XmlString xmlns_xml_uri("http://www.w3.org/XML/1998/namespace");
		static XmlString xmlns_xmlns("xmlns");
		static XmlString xmlns_xmlns_uri("http://www.w3.org/2000/xmlns/");

		XmlString prefix;
		XmlString::size_type pos = qualified_name.find(':');
		if (pos != XmlString::npos)
			prefix = qualified_name.substr(0, pos);

		if (prefix == xmlns_xml)
			return xmlns_xml;
		else if (prefix == xmlns_xmlns || qualified_name == xmlns_xmlns)
			return xmlns_xmlns;

		auto doc_impl = weak_owner_document.lock();
		const XmlTreeNode *cur = get_tree_node();
		while (cur)
		{
			const XmlTreeNode *cur_attr = cur->get_first_attribute(doc_impl.get());
			while (cur_attr)
			{
				std::string node_name = cur_attr->get_node_name();
				if (prefix.empty())
				{
					if (node_name == xmlns_xmlns)
						return cur_attr->get_node_value();
				}
				else
				{
					if (node_name.substr(0, 6) == xmlns_prefix && node_name.substr(6) == prefix)
						return cur_attr->get_node_value();
				}
				cur_attr = cur_attr->get_next_sibling(doc_impl.get());
			}
			cur = cur->get_parent(doc_impl.get());
		}
		return XmlString();
	}

	XmlString XmlNodeImpl::find_prefix(const XmlString &namespace_uri) const
	{
		for (auto cur = shared_from_this(); cur; cur = cur->parent())
		{
			for (auto attr = cur->first_attribute(); attr; attr = attr->next_sibling())
			{
				if (attr->prefix() == "xmlns" && attr->text() == namespace_uri)
				{
					return attr->local_name();
				}
			}
		}
		return XmlString();
	}

	XmlTreeNode *XmlNodeImpl::get_tree_node()
	{
		if (node_index == cl_null_node_index)
			return nullptr;
		auto doc_impl = weak_owner_document.lock();
		return doc_impl->nodes[node_index];
	}

	const XmlTreeNode *XmlNodeImpl::get_tree_node() const
	{
		if (node_index == cl_null_node_index)
			return nullptr;
		auto doc_impl = weak_owner_document.lock();
		return doc_impl->nodes[node_index];
	}
}
