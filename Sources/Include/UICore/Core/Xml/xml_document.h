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

#include "xml_node.h"
#include <vector>

namespace uicore
{
	class IODevice;

	/// XML document tree
	class XmlDocument : public XmlNode
	{
	public:
		static std::shared_ptr<XmlDocument> create();
		static std::shared_ptr<XmlDocument> load(const std::shared_ptr<IODevice> &input, bool eat_whitespace = true);

		virtual std::shared_ptr<XmlNode> document_element() const = 0;

		virtual std::shared_ptr<XmlNode> create_element(const XmlString &tag_name) = 0;
		virtual std::shared_ptr<XmlNode> create_element(const XmlString &namespace_uri, const XmlString &qualified_name) = 0;
		virtual std::shared_ptr<XmlNode> create_fragment() = 0;
		virtual std::shared_ptr<XmlNode> create_text(const XmlString &text) = 0;
		virtual std::shared_ptr<XmlNode> create_comment(const XmlString &data) = 0;
		virtual std::shared_ptr<XmlNode> create_cdata(const XmlString &data) = 0;
		virtual std::shared_ptr<XmlNode> create_attribute(const XmlString &name) = 0;
		virtual std::shared_ptr<XmlNode> create_attribute(const XmlString &namespace_uri, const XmlString &qualified_name) = 0;

		/// \brief Imports a node from another document to this document.
		/// The returned node has no parent. The source node is not altered or removed from the original document; this method creates a new copy of the source node.
		virtual std::shared_ptr<XmlNode> import_node(const std::shared_ptr<XmlNode> &node, bool deep) = 0;

		virtual void save(const std::shared_ptr<IODevice> &output, bool insert_whitespace = true) const = 0;
	};
}
