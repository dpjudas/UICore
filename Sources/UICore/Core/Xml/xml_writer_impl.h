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

#include "UICore/Core/Xml/xml_writer.h"
#include <string>

namespace uicore
{
	class XmlWriterImpl : public XmlWriter
	{
	public:
		XmlWriterImpl(const IODevicePtr &output);

		bool insert_whitespace() const override;
		void set_insert_whitespace(bool enable) override;
		void write(const XmlToken &token) override;

	private:
		uicore::IODevicePtr output;
		bool _insert_whitespace = true;
		int indent = 0;
		std::string str;
		std::string escaped_string;
		bool first_token = true;
		bool single_line_tag = true;

		std::string insert_escapes_fast(const std::string &str);
	};
}
