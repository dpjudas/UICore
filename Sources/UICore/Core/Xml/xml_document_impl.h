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

#pragma once

#include "UICore/Core/Xml/xml_document.h"
#include "block_allocator.h"
#include <vector>
#include <stack>

#define cl_null_node_index 0xffffffff

namespace uicore
{
	class XmlTreeNode;
	class XmlToken;
	class XmlNodeImpl;

	class XmlDocumentImpl : public XmlDocument
	{
	public:
		XmlDocumentImpl();
		~XmlDocumentImpl();

		XmlNodeType type() const override { return XmlNodeType::document; }
		XmlString name() const override { return "#document"; }
		XmlString namespace_uri() const override { return XmlString(); }
		XmlString prefix() const override { return XmlString(); }
		XmlString local_name() const override { return "#document"; }
		XmlString text() const override { return XmlString(); }
		std::shared_ptr<XmlNode> parent() const override { return nullptr; }
		std::shared_ptr<XmlNode> first_child() const override;
		std::shared_ptr<XmlNode> last_child() const override;
		std::shared_ptr<XmlNode> previous_sibling() const override { return nullptr; }
		std::shared_ptr<XmlNode> next_sibling() const override { return nullptr; }
		std::shared_ptr<XmlNode> first_attribute() const override { return nullptr; }
		std::shared_ptr<XmlDocument> owner_document() const override { return std::static_pointer_cast<XmlDocument>(const_cast<XmlDocumentImpl*>(this)->shared_from_this()); }
		bool has_attributes() const override { return false; }
		bool has_attribute(const XmlString &name) const override { return false; }
		bool has_attribute(const XmlString &namespace_uri, const XmlString &local_name) const override { return false; }
		XmlString attribute(const XmlString &name, const XmlString &default_value) const override { return default_value; }
		XmlString attribute_ns(const XmlString &namespace_uri, const XmlString &local_name, const XmlString &default_value) const override { return default_value; }
		std::shared_ptr<XmlNode> attribute_node(const XmlString &name) const override { return nullptr; }
		std::shared_ptr<XmlNode> attribute_node(const XmlString &namespace_uri, const XmlString &local_name) const override { return nullptr; }
		void set_prefix(const XmlString &prefix) override { }
		void set_text(const XmlString &value) override { }
		void set_attribute(const XmlString &name, const XmlString &value) override { }
		void set_attribute(const XmlString &namespace_uri, const XmlString &qualified_name, const XmlString &value) override { }
		void remove_attribute(const XmlString &name) override { }
		void remove_attribute(const XmlString &namespace_uri, const XmlString &local_name) override { }
		std::shared_ptr<XmlNode> remove_attribute(const std::shared_ptr<XmlNode> &old_attribute) override { return old_attribute; }
		std::shared_ptr<XmlNode> add_attribute(const std::shared_ptr<XmlNode> &new_attribute) override { return new_attribute; }
		std::shared_ptr<XmlNode> insert_before(const std::shared_ptr<XmlNode> &new_child, const std::shared_ptr<XmlNode> &ref_child) override;
		std::shared_ptr<XmlNode> replace_child(const std::shared_ptr<XmlNode> &new_child, const std::shared_ptr<XmlNode> &old_child) override;
		std::shared_ptr<XmlNode> remove_child(const std::shared_ptr<XmlNode> &old_child) override;
		std::shared_ptr<XmlNode> append_child(const std::shared_ptr<XmlNode> &new_child) override;
		std::shared_ptr<XmlNode> clone(bool deep) const override;
		std::shared_ptr<XmlNode> named_item(const XmlString &name) const override;
		std::shared_ptr<XmlNode> named_item(const XmlString &namespace_uri, const XmlString &local_name) const override;
		XmlString find_namespace_uri(const XmlString &qualified_name) const override { return XmlString(); }
		XmlString find_prefix(const XmlString &namespace_uri) const override { return XmlString(); }

		std::shared_ptr<XmlNode> document_element() const override;
		std::shared_ptr<XmlNode> create_element(const XmlString &tag_name) override;
		std::shared_ptr<XmlNode> create_element(const XmlString &namespace_uri, const XmlString &qualified_name) override;
		std::shared_ptr<XmlNode> create_fragment() override;
		std::shared_ptr<XmlNode> create_text(const XmlString &text) override;
		std::shared_ptr<XmlNode> create_comment(const XmlString &data) override;
		std::shared_ptr<XmlNode> create_cdata(const XmlString &data) override;
		std::shared_ptr<XmlNode> create_attribute(const XmlString &name) override;
		std::shared_ptr<XmlNode> create_attribute(const XmlString &namespace_uri, const XmlString &qualified_name) override;
		std::shared_ptr<XmlNode> import_node(const std::shared_ptr<XmlNode> &node, bool deep) override;
		void save(const std::shared_ptr<IODevice> &output, bool insert_whitespace = true) const override;

		BlockAllocator node_allocator;
		std::vector<XmlTreeNode *> nodes;
		std::vector<int> free_nodes;
		std::vector<XmlNodeImpl *> free_dom_nodes;

		unsigned int node_index = cl_null_node_index;

		XmlTreeNode *get_tree_node()
		{
			if (node_index == cl_null_node_index)
				return nullptr;
			else
				return nodes[node_index];
		}

		const XmlTreeNode *get_tree_node() const
		{
			if (node_index == cl_null_node_index)
				return nullptr;
			else
				return nodes[node_index];
		}

		static XmlString find_namespace_uri(const XmlString &qualified_name, const XmlToken &search_token, const std::shared_ptr<XmlNode> &search_node);

		unsigned int allocate_tree_node(XmlNodeType type);
		void free_tree_node(unsigned int node_index);

		std::shared_ptr<XmlNodeImpl> allocate_dom_node(unsigned int node_index);
		void free_dom_node(XmlNodeImpl *node);
	};
}
