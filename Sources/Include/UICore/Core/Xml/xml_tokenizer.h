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

#include "xml_token.h"
#include <memory>

namespace uicore
{
	class IODevice;
	typedef std::shared_ptr<IODevice> IODevicePtr;
	class XmlToken;

	/// \brief The XML Tokenizer breaks a XML file into XML tokens.
	class XmlTokenizer
	{
	public:
		/// \brief Constructs a XmlTokenizer
		static std::shared_ptr<XmlTokenizer> create(const IODevicePtr &input);

		/// \brief Returns true if eat whitespace flag is set.
		virtual bool eat_whitespace() const = 0;

		/// \brief If enabled, will eat any whitespace between tags.
		virtual void set_eat_whitespace(bool enable = true) = 0;

		/// \brief Returns the next token available in input stream.
		XmlToken next() { XmlToken token; next(&token); return token; }
		virtual void next(XmlToken *out_token) = 0;
	};

	typedef std::shared_ptr<XmlTokenizer> XmlTokenizerPtr;
}
