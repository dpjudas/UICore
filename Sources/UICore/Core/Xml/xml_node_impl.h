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

#include "UICore/Core/Xml/xml_node.h"
#include "block_allocator.h"
#include "xml_tree_node.h"
#include <map>
#include <memory>

namespace uicore
{
	class XmlTreeNode;
	class XmlDocumentImpl;

	class XmlNodeImpl : public XmlNode
	{
	public:
		XmlNodeType type() const override;
		XmlString name() const override;
		XmlString namespace_uri() const override;
		XmlString prefix() const override;
		XmlString local_name() const override;
		XmlString text() const override;
		XmlNodePtr parent() const override;
		XmlNodePtr first_child() const override;
		XmlNodePtr last_child() const override;
		XmlNodePtr previous_sibling() const override;
		XmlNodePtr next_sibling() const override;
		XmlNodePtr first_attribute() const override;
		XmlDocumentPtr owner_document() const override;
		bool has_attributes() const override;
		bool has_attribute(const XmlString &name) const override;
		bool has_attribute(const XmlString &namespace_uri, const XmlString &local_name) const override;
		XmlString attribute(const XmlString &name, const XmlString &default_value) const override;
		XmlString attribute_ns(const XmlString &namespace_uri, const XmlString &local_name, const XmlString &default_value) const override;
		XmlNodePtr attribute_node(const XmlString &name) const override;
		XmlNodePtr attribute_node(const XmlString &namespace_uri, const XmlString &local_name) const override;
		void set_prefix(const XmlString &prefix) override;
		void set_text(const XmlString &value) override;
		void set_attribute(const XmlString &name, const XmlString &value) override;
		void set_attribute(const XmlString &namespace_uri, const XmlString &qualified_name, const XmlString &value) override;
		XmlNodePtr remove_attribute(const XmlNodePtr &old_attribute) override;
		XmlNodePtr add_attribute(const XmlNodePtr &new_attribute) override;
		void remove_attribute(const XmlString &name) override;
		void remove_attribute(const XmlString &namespace_uri, const XmlString &local_name) override;
		XmlNodePtr insert_before(const XmlNodePtr &new_child, const XmlNodePtr &ref_child) override;
		XmlNodePtr replace_child(const XmlNodePtr &new_child, const XmlNodePtr &old_child) override;
		XmlNodePtr remove_child(const XmlNodePtr &old_child) override;
		XmlNodePtr append_child(const XmlNodePtr &new_child) override;
		XmlNodePtr clone(bool deep) const override;
		XmlNodePtr named_item(const XmlString &name) const override;
		XmlNodePtr named_item(const XmlString &namespace_uri, const XmlString &local_name) const override;
		XmlString find_namespace_uri(const XmlString &qualified_name) const override;
		XmlString find_prefix(const XmlString &namespace_uri) const override;

	private:
		std::weak_ptr<XmlDocumentImpl> weak_owner_document;
		unsigned int node_index = cl_null_node_index;

		XmlTreeNode *get_tree_node();
		const XmlTreeNode *get_tree_node() const;

		friend class XmlDocument;
		friend class XmlDocumentImpl;
	};
}
