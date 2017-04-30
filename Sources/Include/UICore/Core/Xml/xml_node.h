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

#include <memory>
#include <string>

namespace uicore
{
	class XmlNode;
	class XmlDocument;

	typedef std::string XmlString;

	enum class XmlNodeType
	{
		element,
		attribute,
		text,
		cdata,
		comment,
		document,
		document_fragment
	};

	/// Tree node in a XML document
	class XmlNode : public std::enable_shared_from_this<XmlNode>
	{
	public:
		virtual XmlNodeType type() const = 0;
		virtual XmlString name() const = 0;
		virtual XmlString namespace_uri() const = 0;
		virtual XmlString prefix() const = 0;
		virtual XmlString local_name() const = 0;
		virtual XmlString text() const = 0;

		virtual std::shared_ptr<XmlNode> parent() const = 0;
		virtual std::shared_ptr<XmlNode> first_child() const = 0;
		virtual std::shared_ptr<XmlNode> last_child() const = 0;
		virtual std::shared_ptr<XmlNode> previous_sibling() const = 0;
		virtual std::shared_ptr<XmlNode> next_sibling() const = 0;
		virtual std::shared_ptr<XmlNode> first_attribute() const = 0;

		virtual std::shared_ptr<XmlDocument> owner_document() const = 0;

		bool is_element() const { return type() == XmlNodeType::element; }
		bool is_attr() const { return type() == XmlNodeType::attribute; }
		bool is_text() const { return type() == XmlNodeType::text; }
		bool is_cdata_section() const { return type() == XmlNodeType::cdata; }
		bool is_comment() const { return type() == XmlNodeType::comment; }
		bool is_document() const { return type() == XmlNodeType::document; }
		bool is_document_fragment() const { return type() == XmlNodeType::document_fragment; }

		bool has_child_nodes() const { return (bool)first_child(); }

		virtual bool has_attributes() const = 0;
		virtual bool has_attribute(const XmlString &name) const = 0;
		virtual bool has_attribute(const XmlString &namespace_uri, const XmlString &local_name) const = 0;

		virtual XmlString attribute(const XmlString &name, const XmlString &default_value = XmlString()) const = 0;
		virtual XmlString attribute_ns(const XmlString &namespace_uri, const XmlString &local_name, const XmlString &default_value = XmlString()) const = 0;
		virtual std::shared_ptr<XmlNode> attribute_node(const XmlString &name) const = 0;
		virtual std::shared_ptr<XmlNode> attribute_node(const XmlString &namespace_uri, const XmlString &local_name) const = 0;

		virtual void set_prefix(const XmlString &prefix) = 0;
		virtual void set_text(const XmlString &value) = 0;

		virtual void set_attribute(const XmlString &name, const XmlString &value) = 0;
		virtual void set_attribute(const XmlString &namespace_uri, const XmlString &qualified_name, const XmlString &value) = 0;

		virtual void remove_attribute(const XmlString &name) = 0;
		virtual void remove_attribute(const XmlString &namespace_uri, const XmlString &local_name) = 0;
		virtual std::shared_ptr<XmlNode> remove_attribute(const std::shared_ptr<XmlNode> &old_attribute) = 0;
		virtual std::shared_ptr<XmlNode> add_attribute(const std::shared_ptr<XmlNode> &new_attribute) = 0;

		virtual std::shared_ptr<XmlNode> insert_before(const std::shared_ptr<XmlNode> &new_child, const std::shared_ptr<XmlNode> &ref_child) = 0;
		virtual std::shared_ptr<XmlNode> replace_child(const std::shared_ptr<XmlNode> &new_child, const std::shared_ptr<XmlNode> &old_child) = 0;
		virtual std::shared_ptr<XmlNode> remove_child(const std::shared_ptr<XmlNode> &old_child) = 0;
		virtual std::shared_ptr<XmlNode> append_child(const std::shared_ptr<XmlNode> &new_child) = 0;

		/// \brief Returns a duplicate of this node, i.e., serves as a generic copy constructor for nodes.
		/// The duplicate node has no parent.
		/// \param deep If true, recursively clone the subtree under the specified node; if false, clone only the node itself (and its attributes, if it is an Element).
		virtual std::shared_ptr<XmlNode> clone(bool deep) const = 0;

		/// \brief Returns the first child node with the specified node name.
		/// Returns null if no child is found.
		virtual std::shared_ptr<XmlNode> named_item(const XmlString &name) const = 0;
		virtual std::shared_ptr<XmlNode> named_item(const XmlString &namespace_uri, const XmlString &local_name) const = 0;

		/// \brief Searches the node tree upwards for the namespace URI of the given qualified name.
		virtual XmlString find_namespace_uri(const XmlString &qualified_name) const = 0;

		/// \brief Searches the node tree upwards for the prefix name for the namespace URI.
		virtual XmlString find_prefix(const XmlString &namespace_uri) const = 0;
	};
}
