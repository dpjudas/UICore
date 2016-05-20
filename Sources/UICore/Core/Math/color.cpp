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
**    Mark Page
*/

#include "UICore/precomp.h"
#include "UICore/Core/Math/color.h"
#include "UICore/Core/Text/string_format.h"
#include "UICore/Core/Text/text.h"
#include <map>

#ifndef WIN32
#include <cstdlib>
#endif

namespace uicore
{
	Color::Color(const std::string &hexstr)
	{
		if (hexstr.empty() || hexstr.length() > 9)
			throw Exception(string_format("Bad hex color string: %1", hexstr));

		int pos = 0;
		if (hexstr[0] == '#')
			pos++;

		unsigned int color = Text::parse_uint32(hexstr.substr(pos), 16);

		bool have_alpha = (hexstr.length() - pos) > 6;

		if (!have_alpha)
		{
			set_rgb8(color);
		}
		else
		{
			set_rgba8(color);
		}
	}

	void Color::set_rgba8(unsigned int color)
	{
		x = (unsigned char)(color >> 24);
		y = (unsigned char)(color >> 16);
		z = (unsigned char)(color >> 8);
		w = (unsigned char)color;
	}

	void Color::set_bgra8(unsigned int color)
	{
		z = (unsigned char)(color >> 24);
		y = (unsigned char)(color >> 16);
		x = (unsigned char)(color >> 8);
		w = (unsigned char)color;
	}

	void Color::set_rgb8(unsigned int color)
	{
		x = (unsigned char)(color >> 16);
		y = (unsigned char)(color >> 8);
		z = (unsigned char)color;
		w = 0xFF;
	}

	void Color::set_argb8(unsigned int color)
	{
		x = (unsigned char)(color >> 16);
		y = (unsigned char)(color >> 8);
		z = (unsigned char)color;
		w = (unsigned char)(color >> 24);
	}

	Color::Color(const Colorf& color)
		: Vec4ub(static_cast<unsigned char>(color.x * 255.0f),
		static_cast<unsigned char>(color.y * 255.0f),
		static_cast<unsigned char>(color.z * 255.0f),
		static_cast<unsigned char>(color.w * 255.0f))
	{
	}

	Colorf::Colorf(const std::string &hexstr)
	{
		StandardColorf::parse(hexstr, *this);
	}

	bool StandardColorf::parse(const std::string &name, Colorf &out_color)
	{
		if (name.empty())
		{
			out_color = transparent();
			return false;
		}

		if (name[0] == '#')
		{
			out_color = Colorf(Color(name));
			return true;
		}

		static std::map<std::string, Colorf> color_map =
		{
			{ "aliceblue", aliceblue() },
			{ "antiquewhite", antiquewhite() },
			{ "aqua", aqua() },
			{ "aquamarine", aquamarine() },
			{ "azure", azure() },
			{ "beige", beige() },
			{ "bisque", bisque() },
			{ "black", black() },
			{ "blanchedalmond", blanchedalmond() },
			{ "blue", blue() },
			{ "blueviolet", blueviolet() },
			{ "brown", brown() },
			{ "burlywood", burlywood() },
			{ "cadetblue", cadetblue() },
			{ "chartreuse", chartreuse() },
			{ "chocolate", chocolate() },
			{ "coral", coral() },
			{ "cornflowerblue", cornflowerblue() },
			{ "cornsilk", cornsilk() },
			{ "crimson", crimson() },
			{ "cyan", cyan() },
			{ "darkblue", darkblue() },
			{ "darkcyan", darkcyan() },
			{ "darkgoldenrod", darkgoldenrod() },
			{ "darkgray", darkgray() },
			{ "darkgreen", darkgreen() },
			{ "darkgrey", darkgrey() },
			{ "darkkhaki", darkkhaki() },
			{ "darkmagenta", darkmagenta() },
			{ "darkolivegreen", darkolivegreen() },
			{ "darkorange", darkorange() },
			{ "darkorchid", darkorchid() },
			{ "darkred", darkred() },
			{ "darksalmon", darksalmon() },
			{ "darkseagreen", darkseagreen() },
			{ "darkslateblue", darkslateblue() },
			{ "darkslategray", darkslategray() },
			{ "darkslategrey", darkslategrey() },
			{ "darkturquoise", darkturquoise() },
			{ "darkviolet", darkviolet() },
			{ "deeppink", deeppink() },
			{ "deepskyblue", deepskyblue() },
			{ "dimgray", dimgray() },
			{ "dimgrey", dimgrey() },
			{ "dodgerblue", dodgerblue() },
			{ "firebrick", firebrick() },
			{ "floralwhite", floralwhite() },
			{ "forestgreen", forestgreen() },
			{ "fuchsia", fuchsia() },
			{ "gainsboro", gainsboro() },
			{ "ghostwhite", ghostwhite() },
			{ "gold", gold() },
			{ "goldenrod", goldenrod() },
			{ "gray", gray() },
			{ "grey", grey() },
			{ "green", green() },
			{ "greenyellow", greenyellow() },
			{ "honeydew", honeydew() },
			{ "hotpink", hotpink() },
			{ "indianred", indianred() },
			{ "indigo", indigo() },
			{ "ivory", ivory() },
			{ "khaki", khaki() },
			{ "lavender", lavender() },
			{ "lavenderblush", lavenderblush() },
			{ "lawngreen", lawngreen() },
			{ "lemonchiffon", lemonchiffon() },
			{ "lightblue", lightblue() },
			{ "lightcoral", lightcoral() },
			{ "lightcyan", lightcyan() },
			{ "lightgoldenrodyellow", lightgoldenrodyellow() },
			{ "lightgray", lightgray() },
			{ "lightgreen", lightgreen() },
			{ "lightgrey", lightgrey() },
			{ "lightpink", lightpink() },
			{ "lightsalmon", lightsalmon() },
			{ "lightseagreen", lightseagreen() },
			{ "lightskyblue", lightskyblue() },
			{ "lightslategray", lightslategray() },
			{ "lightslategrey", lightslategrey() },
			{ "lightsteelblue", lightsteelblue() },
			{ "lightyellow", lightyellow() },
			{ "lime", lime() },
			{ "limegreen", limegreen() },
			{ "linen", linen() },
			{ "magenta", magenta() },
			{ "maroon", maroon() },
			{ "mediumaquamarine", mediumaquamarine() },
			{ "mediumblue", mediumblue() },
			{ "mediumorchid", mediumorchid() },
			{ "mediumpurple", mediumpurple() },
			{ "mediumseagreen", mediumseagreen() },
			{ "mediumslateblue", mediumslateblue() },
			{ "mediumspringgreen", mediumspringgreen() },
			{ "mediumturquoise", mediumturquoise() },
			{ "mediumvioletred", mediumvioletred() },
			{ "midnightblue", midnightblue() },
			{ "mintcream", mintcream() },
			{ "mistyrose", mistyrose() },
			{ "moccasin", moccasin() },
			{ "navajowhite", navajowhite() },
			{ "navy", navy() },
			{ "oldlace", oldlace() },
			{ "olive", olive() },
			{ "olivedrab", olivedrab() },
			{ "orange", orange() },
			{ "orangered", orangered() },
			{ "orchid", orchid() },
			{ "palegoldenrod", palegoldenrod() },
			{ "palegreen", palegreen() },
			{ "paleturquoise", paleturquoise() },
			{ "palevioletred", palevioletred() },
			{ "papayawhip", papayawhip() },
			{ "peachpuff", peachpuff() },
			{ "peru", peru() },
			{ "pink", pink() },
			{ "plum", plum() },
			{ "powderblue", powderblue() },
			{ "purple", purple() },
			{ "red", red() },
			{ "rosybrown", rosybrown() },
			{ "royalblue", royalblue() },
			{ "saddlebrown", saddlebrown() },
			{ "salmon", salmon() },
			{ "sandybrown", sandybrown() },
			{ "seagreen", seagreen() },
			{ "seashell", seashell() },
			{ "sienna", sienna() },
			{ "silver", silver() },
			{ "skyblue", skyblue() },
			{ "slateblue", slateblue() },
			{ "slategray", slategray() },
			{ "slategrey", slategrey() },
			{ "snow", snow() },
			{ "springgreen", springgreen() },
			{ "steelblue", steelblue() },
			{ "tan", tan() },
			{ "teal", teal() },
			{ "thistle", thistle() },
			{ "transparent", transparent() },
			{ "tomato", tomato() },
			{ "turquoise", turquoise() },
			{ "violet", violet() },
			{ "wheat", wheat() },
			{ "white", white() },
			{ "whitesmoke", whitesmoke() },
			{ "yellow", yellow() },
			{ "yellowgreen", yellowgreen() },
			{ "gray10", gray10() },
			{ "gray20", gray20() },
			{ "gray30", gray30() },
			{ "gray40", gray40() },
			{ "gray50", gray50() },
			{ "gray60", gray60() },
			{ "gray70", gray70() },
			{ "gray80", gray80() },
			{ "gray90", gray90() }
		};

		auto it = color_map.find(name);
		if (it != color_map.end())
		{
			out_color = it->second;
			return true;
		}

		out_color = transparent();
		return false;
	}
}
