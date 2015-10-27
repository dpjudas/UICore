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

#include <vector>
#include <utility>

namespace uicore
{
	enum class XmlTokenType
	{
		null,
		element,
		text,
		cdata,
		processing_instruction,
		comment,
		doctype
	};

	enum class XmlTokenVariant
	{
		begin,
		end,
		single
	};

	/// \brief XML token in a XML file.
	class XmlToken
	{
	public:
		/// Attribute name/value pair.
		typedef std::pair<std::string, std::string> Attribute;

		/// \brief The token type.
		XmlTokenType type = XmlTokenType::null;

		/// \brief The token variant.
		XmlTokenVariant variant = XmlTokenVariant::single;

		/// \brief The name of the token.
		std::string name;

		/// \brief Returns the value of the token.
		std::string value;

		/// \brief All the attributes attached to the token.
		std::vector<Attribute> attributes;
	};
}
