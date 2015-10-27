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
		XmlNodePtr parent() const override { return nullptr; }
		XmlNodePtr first_child() const override { return document_element(); }
		XmlNodePtr last_child() const override { return document_element(); }
		XmlNodePtr previous_sibling() const override { return nullptr; }
		XmlNodePtr next_sibling() const override { return nullptr; }
		XmlNodePtr first_attribute() const override { return nullptr; }
		XmlDocumentPtr owner_document() const override { return std::static_pointer_cast<XmlDocument>(const_cast<XmlDocumentImpl*>(this)->shared_from_this()); }
		bool has_attributes() const override { return false; }
		bool has_attribute(const XmlString &name) const override { return false; }
		bool has_attribute(const XmlString &namespace_uri, const XmlString &local_name) const override { return false; }
		XmlString attribute(const XmlString &name, const XmlString &default_value) const override { return default_value; }
		XmlString attribute_ns(const XmlString &namespace_uri, const XmlString &local_name, const XmlString &default_value) const override { return default_value; }
		XmlNodePtr attribute_node(const XmlString &name) const override { return nullptr; }
		XmlNodePtr attribute_node(const XmlString &namespace_uri, const XmlString &local_name) const override { return nullptr; }
		void set_prefix(const XmlString &prefix) override { }
		void set_text(const XmlString &value) override { }
		void set_attribute(const XmlString &name, const XmlString &value) override { }
		void set_attribute(const XmlString &namespace_uri, const XmlString &qualified_name, const XmlString &value) override { }
		void remove_attribute(const XmlString &name) override { }
		void remove_attribute(const XmlString &namespace_uri, const XmlString &local_name) override { }
		XmlNodePtr remove_attribute(const XmlNodePtr &old_attribute) override { return old_attribute; }
		XmlNodePtr add_attribute(const XmlNodePtr &new_attribute) override { return new_attribute; }
		XmlNodePtr insert_before(const XmlNodePtr &new_child, const XmlNodePtr &ref_child) override;
		XmlNodePtr replace_child(const XmlNodePtr &new_child, const XmlNodePtr &old_child) override;
		XmlNodePtr remove_child(const XmlNodePtr &old_child) override;
		XmlNodePtr append_child(const XmlNodePtr &new_child) override;
		XmlNodePtr clone(bool deep) const override;
		XmlNodePtr named_item(const XmlString &name) const override;
		XmlNodePtr named_item(const XmlString &namespace_uri, const XmlString &local_name) const override;
		XmlString find_namespace_uri(const XmlString &qualified_name) const override { return XmlString(); }
		XmlString find_prefix(const XmlString &namespace_uri) const override { return XmlString(); }

		XmlNodePtr document_element() const override;
		XmlNodePtr create_element(const XmlString &tag_name) override;
		XmlNodePtr create_element(const XmlString &namespace_uri, const XmlString &qualified_name) override;
		XmlNodePtr create_fragment() override;
		XmlNodePtr create_text(const XmlString &text) override;
		XmlNodePtr create_comment(const XmlString &data) override;
		XmlNodePtr create_cdata(const XmlString &data) override;
		XmlNodePtr create_attribute(const XmlString &name) override;
		XmlNodePtr create_attribute(const XmlString &namespace_uri, const XmlString &qualified_name) override;
		XmlNodePtr import_node(const XmlNodePtr &node, bool deep) override;
		void save(const IODevicePtr &output, bool insert_whitespace = true) const override;

		BlockAllocator node_allocator;
		std::vector<XmlTreeNode *> nodes;
		std::vector<int> free_nodes;
		std::vector<XmlNodeImpl *> free_dom_nodes;

		unsigned int node_index = cl_null_node_index;

		static XmlString find_namespace_uri(const XmlString &qualified_name, const XmlToken &search_token, const XmlNodePtr &search_node);

		unsigned int allocate_tree_node(XmlNodeType type);
		void free_tree_node(unsigned int node_index);

		std::shared_ptr<XmlNodeImpl> allocate_dom_node(unsigned int node_index);
		void free_dom_node(XmlNodeImpl *node);
	};
}
