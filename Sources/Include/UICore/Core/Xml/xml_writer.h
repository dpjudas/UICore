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

namespace uicore
{
	class IODevice;
	class XmlToken;

	/// \brief The XML Writer writes a XML file based on XML tokens.
	class XmlWriter
	{
	public:
		/// \brief Constructs a XmlWriter
		static std::shared_ptr<XmlWriter> create(const std::shared_ptr<IODevice> &output);

		/// \brief Returns the insert whitespace flag.
		virtual bool insert_whitespace() const = 0;

		/// \brief Inserts whitespace between tags if enabled.
		virtual void set_insert_whitespace(bool enable = true) = 0;

		/// \brief Write token to file.
		virtual void write(const XmlToken &token) = 0;
	};
}
