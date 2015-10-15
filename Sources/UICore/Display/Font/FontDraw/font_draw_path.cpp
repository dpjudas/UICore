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
**    Mark Page
*/

#include "UICore/precomp.h"
#include "UICore/Display/Font/font.h"
#include "UICore/Display/Font/font_metrics.h"
#include "UICore/Display/Font/font_description.h"
#include "UICore/Core/Text/text.h"
#include "UICore/Core/Text/string_format.h"
#include "UICore/Core/Text/utf8_reader.h"
#include "UICore/Display/2D/canvas.h"
#include "UICore/Display/2D/path.h"
#include "UICore/Core/IOData/path_help.h"
#include "UICore/Display/2D/canvas_impl.h"
#include "UICore/Display/Font/FontEngine/font_engine.h"
#include "font_draw_path.h"
#include "UICore/Display/Font/glyph_cache.h"
#include "UICore/Display/Font/path_cache.h"

namespace uicore
{
	void Font_DrawPath::init(PathCache *cache, FontEngine *engine, float new_scaled_height)
	{
		path_cache = cache;
		font_engine = engine;
		scaled_height = new_scaled_height;
	}

	GlyphMetrics Font_DrawPath::get_metrics(const CanvasPtr &canvas, unsigned int glyph)
	{
		return path_cache->get_metrics(font_engine, canvas, glyph);
	}

	void Font_DrawPath::draw_text(const CanvasPtr &canvas, const Pointf &position, const std::string &text, const Colorf &color, float line_spacing)
	{
		float offset_x = 0;
		float offset_y = 0;
		UTF8_Reader reader(text.data(), text.length());

		const Mat4f original_transform = canvas->transform();
		uicore::Mat4f scale_matrix = uicore::Mat4f::scale(scaled_height, scaled_height, scaled_height);
		Brush brush(color);
		Sizef advance;

		while (!reader.is_end())
		{
			unsigned int glyph = reader.get_char();
			reader.next();

			if (glyph == '\n')
			{
				offset_x = 0;
				offset_y += line_spacing * scaled_height;
				continue;
			}

			canvas->set_transform(original_transform * Mat4f::translate(position.x + offset_x, position.y + offset_y, 0) * scale_matrix);
			Font_PathGlyph *gptr = path_cache->get_glyph(canvas, font_engine, glyph);
			if (gptr)
			{
				gptr->path->fill(canvas, brush);
				offset_x += gptr->metrics.advance.width * scaled_height;
				offset_y += gptr->metrics.advance.height * scaled_height;
			}
		}
		canvas->set_transform(original_transform);
	}
}
