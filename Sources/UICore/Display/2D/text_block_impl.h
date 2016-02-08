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

#include "UICore/Display/Font/font.h"
#include "UICore/Display/Render/graphic_context.h"
#include "UICore/Display/Font/font_metrics.h"
#include "UICore/Display/2D/text_block.h"
#include "UICore/Display/2D/image.h"

namespace uicore
{
	class Canvas;

	class TextBlockImpl : public TextBlock
	{
	public:
		TextBlockImpl();
		~TextBlockImpl();

		void clear() override;
		void add_text(const std::string &text, const FontPtr &font, const Colorf &color, int id) override;
		void add_image(const ImagePtr &image, float baseline_offset, int id) override;
		void add_component(std::shared_ptr<SpanComponent> component, float baseline_offset, int id) override;
		void layout(const CanvasPtr &canvas, float max_width) override;
		void set_position(const Pointf &pos) override { position = pos; }
		Sizef size() const override { return rect().size(); }
		Rectf rect() const override;
		std::vector<Rectf> rect_by_id(int id) const override;
		HitTestResult hit_test(const CanvasPtr &canvas, const Pointf &pos) override;
		void draw_layout(const CanvasPtr &canvas) override;
		void draw_layout_ellipsis(const CanvasPtr &canvas, const Rectf &content_rect) override;
		void set_component_geometry() override;
		Sizef find_preferred_size(const CanvasPtr &canvas) override;
		void set_selection_range(std::string::size_type start, std::string::size_type end) override;
		void set_selection_colors(const Colorf &foreground, const Colorf &background) override;
		void show_cursor() override { cursor_visible = true; }
		void hide_cursor() override { cursor_visible = false; }
		void set_cursor_pos(std::string::size_type pos) override { cursor_pos = pos; }
		void set_cursor_overwrite_mode(bool enable) override { cursor_overwrite_mode = enable; }
		void set_cursor_color(const Colorf &color) override { cursor_color = color; }
		std::string combined_text() const override;
		void set_align(SpanAlign align) override;
		float first_baseline_offset() override;
		float last_baseline_offset() override;

	private:
		bool cursor_visible = false;
		std::string::size_type cursor_pos = 0;
		bool cursor_overwrite_mode = false;
		Colorf cursor_color = Colorf::black;

		struct InlineBlock
		{
			InlineBlock() : start(0), end(0) { }

			unsigned int start, end;
		};

		enum ObjectType
		{
			object_text,
			object_image,
			object_component
		};

		enum FloatType
		{
			float_none,
			float_left,
			float_right
		};

		struct SpanObject
		{
			ObjectType type = object_text;
			FloatType float_type = float_none;

			FontPtr font;
			Colorf color;
			unsigned int start = 0;
			unsigned int end = 0;

			ImagePtr image;
			std::shared_ptr<SpanComponent> component;
			float baseline_offset = 0;

			int id = -1;
		};

		struct LineSegment
		{
			LineSegment() { }

			ObjectType type = object_text;

			FontPtr font;
			Colorf color;
			int start = 0, end = 0;
			float ascender = 0;
			float descender = 0;

			float x_position = 0;
			float width = 0;

			ImagePtr image;
			SpanComponent *component = nullptr;
			float baseline_offset = 0;

			int id = -1;
		};

		struct Line
		{
			Line() { }

			float width = 0;	// Width of the entire line (including spaces)
			float height = 0;
			float ascender = 0;
			std::vector<LineSegment> segments;
		};

		struct TextSizeResult
		{
			TextSizeResult()  { }
			int start = 0, end = 0;
			float width = 0;
			float height = 0;
			float ascender = 0, descender = 0;
			int objects_traversed = 0;
			std::vector<LineSegment> segments;
		};

		struct CurrentLine
		{
			CurrentLine() { }

			std::vector<SpanObject>::size_type object_index = 0;
			Line cur_line;
			float x_position = 0;
			float y_position = 0;
		};

		struct FloatBox
		{
			FloatBox() { }

			Rectf rect;
			ObjectType type = object_image;
			ImagePtr image;
			SpanComponent *component = nullptr;
			int id = 1;
		};

		TextSizeResult find_text_size(const CanvasPtr &canvas, const InlineBlock &block, unsigned int object_index);
		std::vector<InlineBlock> find_text_blocks();
		void layout_lines(const CanvasPtr &canvas, float max_width);
		void layout_text(const CanvasPtr &canvas, std::vector<InlineBlock> blocks, std::vector<InlineBlock>::size_type block_index, CurrentLine &current_line, float max_width);
		void layout_block(CurrentLine &current_line, float max_width, std::vector<InlineBlock> &blocks, std::vector<InlineBlock>::size_type block_index);
		void layout_float_block(CurrentLine &current_line, float max_width);
		void layout_inline_block(CurrentLine &current_line, float max_width, std::vector<InlineBlock> &blocks, std::vector<InlineBlock>::size_type block_index);
		void reflow_line(CurrentLine &current_line, float max_width);
		FloatBox float_box_left(FloatBox float_box, float max_width);
		FloatBox float_box_right(FloatBox float_box, float max_width);
		FloatBox float_box_any(FloatBox box, float max_width, const std::vector<FloatBox> &floats1);
		bool box_fits_on_line(const FloatBox &box, float max_width);
		void place_line_segments(CurrentLine &current_line, TextSizeResult &text_size_result);
		void force_place_line_segments(CurrentLine &current_line, TextSizeResult &text_size_result, float max_width);
		void next_line(CurrentLine &current_line);
		bool is_newline(const InlineBlock &block);
		bool is_whitespace(const InlineBlock &block);
		bool fits_on_line(float x_position, const TextSizeResult &text_size_result, float max_width);
		bool larger_than_line(const TextSizeResult &text_size_result, float max_width);
		void align_justify(float max_width);
		void align_center(float max_width);
		void align_right(float max_width);
		void draw_layout_image(const CanvasPtr &canvas, Line &line, LineSegment &segment, float x, float y);
		void draw_layout_text(const CanvasPtr &canvas, Line &line, LineSegment &segment, float x, float y);

		std::string::size_type sel_start = 0, sel_end = 0;
		Colorf sel_foreground = Colorf::white, sel_background = Colorf::darkslateblue;

		std::string text;
		std::vector<SpanObject> objects;
		std::vector<Line> lines;
		Pointf position;

		std::vector<FloatBox> floats_left, floats_right;

		SpanAlign alignment = SpanAlign::left;

		struct LayoutCache
		{
			LayoutCache() : object_index(-1) { }
			int object_index;
			FontMetrics metrics;
		};
		LayoutCache layout_cache;

		bool is_ellipsis_draw = false;
		Rectf ellipsis_content_rect;
	};
}
