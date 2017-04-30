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
		std::shared_ptr<XmlNode> parent() const override;
		std::shared_ptr<XmlNode> first_child() const override;
		std::shared_ptr<XmlNode> last_child() const override;
		std::shared_ptr<XmlNode> previous_sibling() const override;
		std::shared_ptr<XmlNode> next_sibling() const override;
		std::shared_ptr<XmlNode> first_attribute() const override;
		std::shared_ptr<XmlDocument> owner_document() const override;
		bool has_attributes() const override;
		bool has_attribute(const XmlString &name) const override;
		bool has_attribute(const XmlString &namespace_uri, const XmlString &local_name) const override;
		XmlString attribute(const XmlString &name, const XmlString &default_value) const override;
		XmlString attribute_ns(const XmlString &namespace_uri, const XmlString &local_name, const XmlString &default_value) const override;
		std::shared_ptr<XmlNode> attribute_node(const XmlString &name) const override;
		std::shared_ptr<XmlNode> attribute_node(const XmlString &namespace_uri, const XmlString &local_name) const override;
		void set_prefix(const XmlString &prefix) override;
		void set_text(const XmlString &value) override;
		void set_attribute(const XmlString &name, const XmlString &value) override;
		void set_attribute(const XmlString &namespace_uri, const XmlString &qualified_name, const XmlString &value) override;
		std::shared_ptr<XmlNode> remove_attribute(const std::shared_ptr<XmlNode> &old_attribute) override;
		std::shared_ptr<XmlNode> add_attribute(const std::shared_ptr<XmlNode> &new_attribute) override;
		void remove_attribute(const XmlString &name) override;
		void remove_attribute(const XmlString &namespace_uri, const XmlString &local_name) override;
		std::shared_ptr<XmlNode> insert_before(const std::shared_ptr<XmlNode> &new_child, const std::shared_ptr<XmlNode> &ref_child) override;
		std::shared_ptr<XmlNode> replace_child(const std::shared_ptr<XmlNode> &new_child, const std::shared_ptr<XmlNode> &old_child) override;
		std::shared_ptr<XmlNode> remove_child(const std::shared_ptr<XmlNode> &old_child) override;
		std::shared_ptr<XmlNode> append_child(const std::shared_ptr<XmlNode> &new_child) override;
		std::shared_ptr<XmlNode> clone(bool deep) const override;
		std::shared_ptr<XmlNode> named_item(const XmlString &name) const override;
		std::shared_ptr<XmlNode> named_item(const XmlString &namespace_uri, const XmlString &local_name) const override;
		XmlString find_namespace_uri(const XmlString &qualified_name) const override;
		XmlString find_prefix(const XmlString &namespace_uri) const override;

	private:
		std::shared_ptr<XmlDocumentImpl> _owner_document;
		unsigned int node_index = cl_null_node_index;

		XmlTreeNode *get_tree_node();
		const XmlTreeNode *get_tree_node() const;

		friend class XmlDocument;
		friend class XmlDocumentImpl;
	};
}
