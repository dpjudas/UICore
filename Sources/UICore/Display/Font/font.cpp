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

#include "UICore/precomp.h"
#include "UICore/Display/Font/font.h"
#include "UICore/Display/Font/font_metrics.h"
#include "UICore/Display/Font/font_description.h"
#include "UICore/Display/Font/font_family.h"
#include "UICore/Core/IOData/path_help.h"
#include "UICore/Core/Text/text.h"
#include "UICore/Core/Text/string_format.h"
#include "UICore/Core/Text/utf8_reader.h"
#include "UICore/Display/2D/canvas.h"
#include "font_impl.h"

namespace uicore
{
	std::shared_ptr<Font> Font::create(const std::shared_ptr<FontFamily> &font_family, const FontDescription &desc)
	{
		return std::make_shared<Font_Impl>(font_family, desc);
	}

	std::shared_ptr<Font> Font::create(const std::shared_ptr<FontFamily> &font_family, float height)
	{
		FontDescription desc;
		desc.set_height(height);
		return create(font_family, desc);
	}

	std::shared_ptr<Font> Font::create(const std::string &typeface_name, float height)
	{
		FontDescription desc;
		desc.set_height(height);
		return create(typeface_name, desc);
	}

	std::shared_ptr<Font> Font::create(const std::string &typeface_name, const FontDescription &desc)
	{
		auto font_family = FontFamily::create(typeface_name);
		return create(font_family, desc);
	}

	std::shared_ptr<Font> Font::create(const FontDescription &desc, const std::string &ttf_filename)
	{
		std::string new_filename = FilePath::filename_without_extension(ttf_filename);

		auto font_family = FontFamily::create(new_filename);
		font_family->add(desc, ttf_filename);
		return create(font_family, desc);
	}

	size_t Font::clip_from_left(const std::shared_ptr<Canvas> &canvas, const std::string &text, float width)
	{
		float x = 0.0f;
		UTF8_Reader reader(text.data(), text.length());
		while (!reader.is_end())
		{
			unsigned int glyph = reader.character();
			GlyphMetrics char_abc = metrics(canvas, glyph);

			if (x + char_abc.advance.width > width)
				return reader.position();

			x += char_abc.advance.width;
			reader.next();
		}

		return text.size();
	}

	size_t Font::clip_from_right(const std::shared_ptr<Canvas> &canvas, const std::string &text, float width)
	{
		float x = 0.0f;
		UTF8_Reader reader(text.data(), text.length());
		reader.set_position(text.length());
		while (reader.position() != 0)
		{
			reader.prev();

			unsigned int glyph = reader.character();
			GlyphMetrics char_abc = metrics(canvas, glyph);

			if (x + char_abc.advance.width > width)
			{
				reader.next();
				return reader.position();
			}

			x += char_abc.advance.width;
		}

		return 0;
	}

	std::string Font::clipped_text(const std::shared_ptr<Canvas> &canvas, const Sizef &box_size, const std::string &text, const std::string &ellipsis_text)
	{
		std::string out_string;
		out_string.reserve(text.length());

		Pointf pos;

		FontMetrics fm = font_metrics(canvas);
		float descent = fm.descent();
		float line_spacing = fm.line_height();
		std::vector<std::string> lines = Text::split(text, "\n", false);
		for (std::vector<std::string>::size_type i = 0; i < lines.size(); i++)
		{
			if (i == 0 || pos.y + descent < box_size.height)
			{
				Sizef size = measure_text(canvas, lines[i]).bbox_size;
				if (pos.x + size.width <= box_size.width)
				{
					if (!out_string.empty())
						out_string += "\n";
					out_string += lines[i];
				}
				else
				{
					Sizef ellipsis = measure_text(canvas, ellipsis_text).bbox_size;

					int seek_start = 0;
					int seek_end = lines[i].size();
					int seek_center = (seek_start + seek_end) / 2;

					UTF8_Reader utf8_reader(lines[i].data(), lines[i].length());
					while (true)
					{
						utf8_reader.set_position(seek_center);
						utf8_reader.move_to_leadbyte();
						if (seek_center != utf8_reader.position())
							utf8_reader.next();
						seek_center = utf8_reader.position();
						if (seek_center == seek_end)
							break;

						utf8_reader.set_position(seek_start);
						utf8_reader.next();
						if (utf8_reader.position() == seek_end)
							break;

						Sizef text_size = measure_text(canvas, lines[i].substr(0, seek_center)).bbox_size;

						if (pos.x + text_size.width + ellipsis.width >= box_size.width)
							seek_end = seek_center;
						else
							seek_start = seek_center;
						seek_center = (seek_start + seek_end) / 2;
					}

					if (!out_string.empty())
						out_string += "\n";
					out_string += lines[i].substr(0, seek_center) + ellipsis_text;
				}
				pos.y += line_spacing;
			}
		}

		return out_string;
	}
}
