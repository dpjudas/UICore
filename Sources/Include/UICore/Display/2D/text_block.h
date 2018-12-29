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

#include <memory>
#include "../../Core/Math/rect.h"
#include "../../Core/Math/size.h"
#include "../../Core/Math/color.h"

namespace uicore
{
	class Font;
	class Pointf;
	class Sizef;
	class Image;
	class Canvas;

	/// \brief Span Align
	enum class SpanAlign
	{
		left,
		right,
		center,
		justify
	};

	/// \brief Component placed in the layout
	class SpanComponent
	{
	public:
		/// \brief Size of component
		virtual Sizef size() const = 0;

		/// \brief Sets the component geometry after layout
		virtual void set_geometry(const Rectf &geometry) = 0;
	};

	/// \brief Span layout class
	class TextBlock
	{
	public:
		static std::shared_ptr<TextBlock> create();

		struct HitTestResult
		{
			enum Type
			{
				no_objects_available,
				outside_top,
				outside_left,
				outside_right,
				outside_bottom,
				inside
			};

			Type type = no_objects_available;
			int object_id = -1;
			int offset = 0;
		};

		/// \brief Removes all objects
		virtual void clear() = 0;

		/// \brief Add text object
		virtual void add_text(const std::string &text, const std::shared_ptr<Font> &font, const Colorf &color = StandardColorf::white(), int id = -1) = 0;

		/// \brief Add image object
		virtual void add_image(const std::shared_ptr<Image> &image, float baseline_offset = 0.0f, int id = -1) = 0;

		/// \brief Add component object
		virtual void add_component(std::shared_ptr<SpanComponent> component, float baseline_offset = 0.0f, int id = -1) = 0;

		/// \brief Layout objects
		virtual void layout(const std::shared_ptr<Canvas> &canvas, float max_width) = 0;

		/// \brief Set position of layout
		virtual void set_position(const Pointf &pos) = 0;

		/// \brief Returns the size of the layout
		virtual Sizef size() const = 0;

		/// \brief Returns the geometry of the layout
		virtual Rectf rect() const = 0;

		/// \brief Returns the geometry of the object with the given id
		virtual std::vector<Rectf> rect_by_id(int id) const = 0;

		/// \brief Hit test which object is located at the specified position
		virtual HitTestResult hit_test(const std::shared_ptr<Canvas> &canvas, const Pointf &pos) = 0;

		/// \brief Draw layout
		virtual void draw_layout(const std::shared_ptr<Canvas> &canvas) = 0;

		/// \brief Draw layout generating ellipsis for clipped text
		virtual void draw_layout_ellipsis(const std::shared_ptr<Canvas> &canvas, const Rectf &content_rect) = 0;

		/// \brief Set component geometry
		virtual void set_component_geometry() = 0;

		/// \brief Find the preferred size for the given layout
		virtual Sizef find_preferred_size(const std::shared_ptr<Canvas> &canvas) = 0;

		/// \brief Set selection range
		virtual void set_selection_range(std::string::size_type start, std::string::size_type end) = 0;

		/// \brief Set selection colors
		virtual void set_selection_colors(const Colorf &foreground, const Colorf &background) = 0;

		/// \brief Shows the cursor caret
		virtual void show_cursor() = 0;

		/// \brief Hides the cursor caret
		virtual void hide_cursor() = 0;

		/// \brief Sets the cursor position
		///
		/// \param pos = Index of cursor
		virtual void set_cursor_pos(std::string::size_type pos) = 0;

		/// \brief Toggles whether the cursor caret is shown as a solid box or a line
		///
		/// \param enable = Shows the cursor as a solid box if set to true
		virtual void set_cursor_overwrite_mode(bool enable) = 0;

		/// \brief Sets the cursor color
		///
		/// \param color = Color of cursor
		virtual void set_cursor_color(const Colorf &color) = 0;

		/// \brief Get all text
		virtual std::string combined_text() const = 0;

		/// \brief Sets the text alignment
		///
		/// Alignment is applied when layout() is called
		virtual void set_align(SpanAlign align) = 0;

		/// \brief Returns the baseline offset for the first baseline
		virtual float first_baseline_offset() = 0;

		/// \brief Returns the baseline offset for the last baseline
		virtual float last_baseline_offset() = 0;
	};
}
