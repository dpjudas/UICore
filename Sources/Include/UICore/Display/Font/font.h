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

#include <memory>
#include "UICore/Display/Render/graphic_context.h"
#include "../Image/pixel_buffer.h"
#include "font_description.h"
#include "glyph_metrics.h"

namespace uicore
{
	class FontProvider;
	class FontFamily;
	typedef std::shared_ptr<FontFamily> FontFamilyPtr;
	class Canvas;
	typedef std::shared_ptr<Canvas> CanvasPtr;
	class GlyphMetrics;

	class FontHandle
	{
	public:
		virtual ~FontHandle() { }
	};

	/// \brief Font class
	///
	/// A Font is a collection of images that can be used to represent text on a screen.
	class Font
	{
	public:
		// \brief Create font using the specified font family
		static std::shared_ptr<Font> create(const FontFamilyPtr &font_family, float height);

		// \brief Create font using the specified font family
		static std::shared_ptr<Font> create(const FontFamilyPtr &font_family, const FontDescription &desc);

		/// \brief Constructs standard font
		static std::shared_ptr<Font> create(const std::string &typeface_name, float height);

		// \brief Constructs standard font
		static std::shared_ptr<Font> create(const std::string &typeface_name, const FontDescription &desc);

		// \brief Constructs standard font
		static std::shared_ptr<Font> create(const FontDescription &desc, const std::string &ttf_filename);

		/// \brief Sets the font height
		virtual void set_height(float value) = 0;

		/// \brief Sets the font weight
		virtual void set_weight(FontWeight value = FontWeight::normal) = 0;

		/// \brief Sets the distance between each line
		virtual void set_line_height(float height) = 0;

		/// \brief Sets the font style setting
		virtual void set_style(FontStyle setting = FontStyle::normal) = 0;

		/// \brief Sets the threshold to determine if the font can be drawn scaled
		///
		/// All font sizes are scalable when using sprite fonts
		virtual void set_scalable(float height_threshold = 64.0f) = 0;

		/// \brief Print text
		///
		/// \param canvas = Canvas
		/// \param position = Dest position
		/// \param text = The text to draw
		/// \param color = The text color
		virtual void draw_text(const CanvasPtr &canvas, const Pointf &position, const std::string &text, const Colorf &color = StandardColorf::white()) = 0;
		void draw_text(const CanvasPtr &canvas, float xpos, float ypos, const std::string &text, const Colorf &color = StandardColorf::white()) { draw_text(canvas, Pointf(xpos, ypos), text, color); }

		/// \brief Gets the glyph metrics
		///
		/// \param glyph = The glyph to get
		/// \return The glyph metrics
		virtual GlyphMetrics metrics(const CanvasPtr &canvas, unsigned int glyph) = 0;

		/// \brief Measure text size
		///
		/// \param string = The text to use
		/// \return The metrics
		virtual GlyphMetrics measure_text(const CanvasPtr &canvas, const std::string &string) = 0;

		/// \brief Retrieves font metrics description for the selected font.
		virtual const FontMetrics &font_metrics(const CanvasPtr &canvas) = 0;

		/// \brief Retrieves clipped version of the text that will fit into a box
		///
		/// \return The string
		std::string clipped_text(const CanvasPtr &canvas, const Sizef &box_size, const std::string &text, const std::string &ellipsis_text = "...");

		/// \brief Get the character index at a specified point
		///
		/// \param canvas = Canvas
		/// \param text = The string
		/// \param point = The point
		/// \return The character index. -1 = Not at specified point
		virtual int character_index(const CanvasPtr &canvas, const std::string &text, const Pointf &point) = 0;

		/// \brief Get the rectangles of each glyph in a string of text
		///
		/// \return A list of Rects for every glyph
		virtual std::vector<Rectf> character_indices(const CanvasPtr &canvas, const std::string &text) = 0;

		// Finds the offset for the last visible character when clipping the head
		size_t clip_from_left(const CanvasPtr &canvas, const std::string &text, float width);

		// Finds the offset for the first visible character when clipping the tail
		size_t clip_from_right(const CanvasPtr &canvas, const std::string &text, float width);

		/// \brief Get the font handle interface
		///
		/// For example, use auto handle = dynamic_cast<FontHandle_Win32>(font.handle()); if (handle) {...} to obtain a specific interface
		///
		/// \return The font handle interface
		virtual FontHandle *handle(const CanvasPtr &canvas) = 0;
	};

	typedef std::shared_ptr<Font> FontPtr;

	#ifdef WIN32
	class FontHandle_Win32 : public FontHandle
	{
	public:
		/// \brief Get the font HFONT
		HFONT hfont();
	private:
		friend class FontEngine_Win32;
		FontEngine_Win32 *engine = nullptr;
	};
	#endif
}
