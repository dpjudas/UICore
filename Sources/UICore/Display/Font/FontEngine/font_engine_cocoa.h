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

#pragma once

#include "font_engine.h"
#include "UICore/Core/System/databuffer.h"
#include "UICore/Display/Font/font.h"
#include "UICore/Display/Font/font_description.h"
#include "UICore/Display/Font/font_metrics.h"
#include "UICore/Display/2D/path.h"
#include <CoreText/CoreText.h>
#include <CoreGraphics/CoreGraphics.h>
#include <string>

namespace uicore
{
	class FontEngine_Cocoa : public FontEngine
	{
	public:
		FontEngine_Cocoa(const FontDescription &description, const std::string &typeface_name, float pixel_ratio);
		FontEngine_Cocoa(const FontDescription &description, DataBufferPtr &font_databuffer, float pixel_ratio);
		~FontEngine_Cocoa();

		bool is_automatic_recreation_allowed() const override { return true; }
		const FontMetrics &get_metrics() const override { return font_metrics; }
		const FontDescription &get_desc() const override { return font_description; }
		FontPixelBuffer get_font_glyph(int glyph) override;

		void load_glyph_path(unsigned int glyph_index, const PathPtr &out_path, GlyphMetrics &out_metrics) override;

	private:
		static Pointf to_pointf(const CGPoint &p);
		
		struct LoadGlyphPathInfo
		{
			LoadGlyphPathInfo(const PathPtr &path, float pixel_ratio)
			: path(path), pixel_ratio(pixel_ratio) { }
			
			const PathPtr &path;
			float pixel_ratio;
		};
		
		CTFontRef handle = 0;
		FontDescription font_description;
		FontMetrics font_metrics;
		float pixel_ratio = 1.0f;
	};
}
