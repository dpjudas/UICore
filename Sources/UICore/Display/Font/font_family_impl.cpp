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

#include "UICore/precomp.h"
#include "font_family_impl.h"
#include "UICore/Display/Font/font_family.h"
#include "UICore/Display/Font/font_metrics.h"
#include "UICore/Display/Font/font_description.h"
#include "UICore/Core/Text/text.h"
#include "UICore/Core/Text/string_format.h"
#include "UICore/Core/Text/utf8_reader.h"
#include "UICore/Display/2D/canvas.h"
#include "UICore/Display/2D/path.h"
#include "UICore/Core/IOData/path_help.h"
#include "UICore/Display/2D/canvas_impl.h"

#ifdef WIN32
#include "FontEngine/font_engine_win32.h"
#elif defined(__APPLE__)
#include "FontEngine/font_engine_cocoa.h"
#else
#include "FontEngine/font_engine_freetype.h"

#if !defined __ANDROID__
#include "../Platform/X11/font_config.h"
#endif

#endif

namespace uicore
{
	class FontEngine_Sprite : public FontEngine
	{
	public:
		FontEngine_Sprite(const FontDescription &desc, FontMetrics &metrics) : font_metrics(metrics) { font_description = desc.clone(); }
		~FontEngine_Sprite() {}

		bool is_automatic_recreation_allowed() const override { return false; }
		const FontMetrics &get_metrics() const override { return font_metrics; }
		FontPixelBuffer get_font_glyph(int glyph) override { return FontPixelBuffer(); }
		void load_glyph_path(unsigned int glyph_index, Path &out_path, GlyphMetrics &out_metrics) override {}
		const FontDescription &get_desc() const override { return font_description; }

	private:
		FontDescription font_description;
		FontMetrics font_metrics;
	};

	FontFamily_Impl::FontFamily_Impl(const std::string &family_name) : family_name(family_name), texture_group(TextureGroup::create(Size(256, 256)))
	{
	}

	FontFamily_Impl::~FontFamily_Impl()
	{
	}

	void FontFamily_Impl::add(const FontDescription &desc, DataBufferPtr &font_databuffer)
	{
		FontFamily_Definition definition;
		definition.desc = desc.clone();
		definition.font_databuffer = font_databuffer;
		font_definitions.push_back(definition);
	}

	void FontFamily_Impl::add(const FontDescription &desc, const std::string &typeface_name)
	{
		FontFamily_Definition definition;
		definition.desc = desc.clone();
		definition.typeface_name = typeface_name;
		font_definitions.push_back(definition);
	}

	void FontFamily_Impl::font_face_load(const FontDescription &desc, DataBufferPtr &font_databuffer, float pixel_ratio)
	{
#if defined(WIN32)
		std::shared_ptr<FontEngine> engine = std::make_shared<FontEngine_Win32>(desc, font_databuffer, pixel_ratio);
		font_cache.push_back(Font_Cache(engine));
#elif defined(__APPLE__)
		std::shared_ptr<FontEngine> engine = std::make_shared<FontEngine_Cocoa>(desc, font_databuffer, pixel_ratio);
		font_cache.push_back(Font_Cache(engine));
#else
		std::shared_ptr<FontEngine> engine = std::make_shared<FontEngine_Freetype>(desc, font_databuffer, pixel_ratio);
		font_cache.push_back(Font_Cache(engine));
#endif
		font_cache.back().glyph_cache->set_texture_group(texture_group);
		font_cache.back().pixel_ratio = pixel_ratio;
	}

	void FontFamily_Impl::font_face_load(const FontDescription &desc, const std::string &typeface_name, float pixel_ratio)
	{
#if defined(WIN32)
		std::shared_ptr<FontEngine> engine = std::make_shared<FontEngine_Win32>(desc, typeface_name, pixel_ratio);
		font_cache.push_back(Font_Cache(engine));
		font_cache.back().glyph_cache->set_texture_group(texture_group);
		font_cache.back().pixel_ratio = pixel_ratio;
#elif defined(__APPLE__)
		std::shared_ptr<FontEngine> engine = std::make_shared<FontEngine_Cocoa>(desc, typeface_name, pixel_ratio);
		font_cache.push_back(Font_Cache(engine));
		font_cache.back().glyph_cache->set_texture_group(texture_group);
		font_cache.back().pixel_ratio = pixel_ratio;
#elif defined(__ANDROID__)
		throw Exception("automatic typeface to ttf file selection is not supported on android");
#else

		// Obtain the best matching font file from fontconfig.
		FontConfig &fc = FontConfig::instance();
		std::string font_file_path = fc.match_font(typeface_name, desc);
		auto font_databuffer = File::read_all_bytes(font_file_path);
		font_face_load(desc, font_databuffer, pixel_ratio);
#endif
	}

	Font_Cache FontFamily_Impl::get_font(const FontDescription &desc, float pixel_ratio)
	{
		// Find cached version
		for (auto &cache : font_cache)
		{
			if (cache.pixel_ratio != pixel_ratio)
				continue;
			if (desc.get_style() != cache.engine->get_desc().get_style())
				continue;
			if (desc.get_weight() != cache.engine->get_desc().get_weight())
				continue;
			if (desc.get_subpixel() != cache.engine->get_desc().get_subpixel())
				continue;
			if (desc.get_anti_alias() != cache.engine->get_desc().get_anti_alias())
				continue;

			if (cache.engine->is_automatic_recreation_allowed())
			{
				if (desc.get_height() != cache.engine->get_desc().get_height())
					continue;
			}

			return cache;
		}
		return Font_Cache();
	}

	Font_Cache FontFamily_Impl::copy_font(const FontDescription &desc, float pixel_ratio)
	{
		// Find existing typeface, to obtain shared data that we can copy
		FontFamily_Definition font_definition;
		bool found = false;

		// Find find an exact match using style and weight
		for (auto &definitions : font_definitions)
		{
			if (desc.get_style() != definitions.desc.get_style())
				continue;
			if (desc.get_weight() != definitions.desc.get_weight())
				continue;

			font_definition = definitions;
			found = true;
			break;
		}

		// Else find the first font
		if (!found)
		{
			if (!font_definitions.empty())
			{
				font_definition = font_definitions[0];
				found = true;
			}
		}

		if (!found)
		{
			// Could not find a cached version of the font to use as reference
			font_face_load(desc, family_name, pixel_ratio);
		}
		else
		{
			if (font_definition.font_databuffer)
			{
				// Cached font is allocated via a font databuffer
				font_face_load(desc, font_definition.font_databuffer, pixel_ratio);
			}
			else
			{
				// Cached font has allocated the typeface_name
				font_face_load(desc, font_definition.typeface_name, pixel_ratio);
			}
		}

		return font_cache.back();
	}
}
