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
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Harry Storbacka
**    Magnus Norddahl
**    Mark Page
*/

#pragma once

#include "UICore/Display/Font/font_metrics.h"
#include "UICore/Display/Font/glyph_metrics.h"
#include "UICore/Display/Font/font.h"
#include "UICore/Display/Font/font_family.h"
#include "UICore/Display/Render/texture_2d.h"
#include <list>
#include <map>
#include "glyph_cache.h"
#include "path_cache.h"
#include "font_family_impl.h"

#include "FontDraw/font_draw_subpixel.h"
#include "FontDraw/font_draw_flat.h"
#include "FontDraw/font_draw_path.h"
#include "FontDraw/font_draw_scaled.h"

namespace uicore
{
	class FontEngine;
	class XMLResourceNode;
	class DomElement;

	class Font_Impl : public Font
	{
	public:
		Font_Impl(const FontFamilyPtr &new_font_family, const FontDescription &description);
		~Font_Impl();

		void set_height(float value) override;
		void set_weight(FontWeight value) override;
		void set_line_height(float height) override;
		void set_style(FontStyle setting) override;
		void set_scalable(float height_threshold) override;
		void draw_text(const CanvasPtr &canvas, const Pointf &position, const std::string &text, const Colorf &color) override;
		GlyphMetrics metrics(const CanvasPtr &canvas, unsigned int glyph) override;
		GlyphMetrics measure_text(const CanvasPtr &canvas, const std::string &string) override;
		const FontMetrics &font_metrics(const CanvasPtr &canvas) override;
		int character_index(const CanvasPtr &canvas, const std::string &text, const Pointf &point) override;
		std::vector<Rectf> character_indices(const CanvasPtr &canvas, const std::string &text) override;
		FontHandle *handle(const CanvasPtr &canvas) override;

		void glyph_path(const CanvasPtr &canvas, unsigned int glyph_index, const PathPtr &out_path, GlyphMetrics &out_metrics);

	private:
		void select_font_family(const CanvasPtr &canvas);

		FontDescription selected_description;
		float selected_line_height = 0.0f;
		float selected_pixel_ratio = 1.0f;
		float scaled_height = 1.0f;
		float selected_height_threshold = 64.0f;		// Values greater or equal to this value can be drawn scaled
		bool selected_pathfont = false;

		FontMetrics selected_metrics;

		FontEngine *font_engine = nullptr;	// If null, use select_font_family() to update
		std::shared_ptr<FontFamily_Impl> font_family;

		Font_Draw *font_draw = nullptr;

		Font_DrawSubPixel font_draw_subpixel;
		Font_DrawFlat font_draw_flat;
		Font_DrawScaled font_draw_scaled;
		Font_DrawPath font_draw_path;
	};
}
