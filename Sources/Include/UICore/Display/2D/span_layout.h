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
	class GraphicContext;
	class Pointf;
	class Sizef;
	class SpanLayout_Impl;
	class Image;
	typedef std::shared_ptr<Image> ImagePtr;
	class Canvas;
	typedef std::shared_ptr<Canvas> CanvasPtr;

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
	class SpanLayout
	{
	public:
		SpanLayout();
		~SpanLayout();

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
		void clear();

		/// \brief Add text object
		void add_text(const std::string &text, const Font &font, const Colorf &color = Colorf::white, int id = -1);

		/// \brief Add image object
		void add_image(const ImagePtr &image, float baseline_offset = 0.0f, int id = -1);

		/// \brief Add component object
		void add_component(std::shared_ptr<SpanComponent> component, float baseline_offset = 0.0f, int id = -1);

		/// \brief Layout objects
		void layout(const CanvasPtr &canvas, float max_width);

		/// \brief Set position of layout
		void set_position(const Pointf &pos);

		/// \brief Returns the size of the layout
		Sizef size() const;

		/// \brief Returns the geometry of the layout
		Rectf rect() const;

		/// \brief Returns the geometry of the object with the given id
		std::vector<Rectf> rect_by_id(int id) const;

		/// \brief Hit test which object is located at the specified position
		HitTestResult hit_test(const CanvasPtr &canvas, const Pointf &pos);

		/// \brief Draw layout
		void draw_layout(const CanvasPtr &canvas);

		/// \brief Draw layout generating ellipsis for clipped text
		void draw_layout_ellipsis(const CanvasPtr &canvas, const Rectf &content_rect);

		/// \brief Set component geometry
		void set_component_geometry();

		/// \brief Find the preferred size for the given layout
		Sizef find_preferred_size(const CanvasPtr &canvas);

		/// \brief Set selection range
		void set_selection_range(std::string::size_type start, std::string::size_type end);

		/// \brief Set selection colors
		void set_selection_colors(const Colorf &foreground, const Colorf &background);

		/// \brief Shows the cursor caret
		void show_cursor();

		/// \brief Hides the cursor caret
		void hide_cursor();

		/// \brief Sets the cursor position
		///
		/// \param pos = Index of cursor
		void set_cursor_pos(std::string::size_type pos);

		/// \brief Toggles whether the cursor caret is shown as a solid box or a line
		///
		/// \param enable = Shows the cursor as a solid box if set to true
		void set_cursor_overwrite_mode(bool enable);

		/// \brief Sets the cursor color
		///
		/// \param color = Color of cursor
		void set_cursor_color(const Colorf &color);

		/// \brief Get all text
		std::string combined_text() const;

		/// \brief Sets the text alignment
		///
		/// Alignment is applied when layout() is called
		void set_align(SpanAlign align);

		/// \brief Returns the baseline offset for the first baseline
		float first_baseline_offset();

		/// \brief Returns the baseline offset for the last baseline
		float last_baseline_offset();

	private:
		std::shared_ptr<SpanLayout_Impl> impl;
	};
}
