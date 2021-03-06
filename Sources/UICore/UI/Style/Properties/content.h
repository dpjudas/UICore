/*
**  UICore
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

#include "UICore/UI/Style/style_property_parser.h"

namespace uicore
{
	void force_link_style_parser_content();

	class BoxSizingPropertyParser : public StylePropertyParser
	{
	public:
		BoxSizingPropertyParser() : StylePropertyParser({ "box-sizing" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class WidthPropertyParser : public StylePropertyParser
	{
	public:
		WidthPropertyParser() : StylePropertyParser({ "width" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class HeightPropertyParser : public StylePropertyParser
	{
	public:
		HeightPropertyParser() : StylePropertyParser({ "height" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class MinWidthPropertyParser : public StylePropertyParser
	{
	public:
		MinWidthPropertyParser() : StylePropertyParser({ "min-width" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class MinHeightPropertyParser : public StylePropertyParser
	{
	public:
		MinHeightPropertyParser() : StylePropertyParser({ "min-height" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class MaxWidthPropertyParser : public StylePropertyParser
	{
	public:
		MaxWidthPropertyParser() : StylePropertyParser({ "max-width" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class MaxHeightPropertyParser : public StylePropertyParser
	{
	public:
		MaxHeightPropertyParser() : StylePropertyParser({ "max-height" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};
}
