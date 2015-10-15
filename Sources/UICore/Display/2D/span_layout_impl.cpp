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
#include "UICore/Core/Math/cl_math.h"
#include "UICore/Display/2D/canvas.h"
#include "UICore/Display/2D/path.h"
#include "UICore/Display/2D/brush.h"
#include "span_layout_impl.h"

namespace uicore
{
	SpanLayoutImpl::SpanLayoutImpl()
	{
	}

	SpanLayoutImpl::~SpanLayoutImpl()
	{
		clear();
	}

	void SpanLayoutImpl::clear()
	{
		objects.clear();
		text.clear();
		lines.clear();
	}

	std::vector<Rectf> SpanLayoutImpl::rect_by_id(int id) const
	{
		std::vector<Rectf> segment_rects;

		float x = position.x;
		float y = position.y;
		for (auto & elem : lines)
		{
			const Line &line = elem;
			for (std::vector<LineSegment>::size_type segment_index = 0; segment_index < line.segments.size(); segment_index++)
			{
				const LineSegment &segment = line.segments[segment_index];
				if (segment.id == id)
				{
					segment_rects.push_back(Rectf(x + segment.x_position, y, segment.width, y + line.height));
				}
			}
			y += line.height;
		}

		return segment_rects;
	}

	void SpanLayoutImpl::draw_layout(const CanvasPtr &canvas)
	{
		float x = position.x;
		float y = position.y;
		for (std::vector<Line>::size_type line_index = 0; line_index < lines.size(); line_index++)
		{
			Line &line = lines[line_index];
			for (std::vector<LineSegment>::size_type segment_index = 0; segment_index < line.segments.size(); segment_index++)
			{
				LineSegment &segment = line.segments[segment_index];
				switch (segment.type)
				{
				case object_text:
					draw_layout_text(canvas, line, segment, x, y);
					break;
				case object_image:
					draw_layout_image(canvas, line, segment, x, y);
					break;
				case object_component:
					break;
				}

			}

			if (line_index + 1 == lines.size() && !line.segments.empty())
			{
				LineSegment &segment = line.segments.back();
				if (cursor_visible && segment.end <= cursor_pos)
				{
					if (segment.type == object_text)
					{
						float cursor_x = x + segment.x_position + segment.font->measure_text(canvas, text.substr(segment.start, segment.end - segment.start)).bbox_size.width;
						float cursor_width = 1.0f;
						Path::rect(cursor_x, y + line.ascender - segment.ascender, cursor_width, segment.ascender + segment.descender)->fill(canvas, cursor_color);
					}
				}
			}

			y += line.height;
		}
	}

	void SpanLayoutImpl::draw_layout_ellipsis(const CanvasPtr &canvas, const Rectf &content_rect)
	{
		is_ellipsis_draw = true;
		ellipsis_content_rect = content_rect;
		try
		{
			is_ellipsis_draw = false;
			draw_layout(canvas);
		}
		catch (...)
		{
			is_ellipsis_draw = false;
			throw;
		}
	}

	void SpanLayoutImpl::draw_layout_image(const CanvasPtr &canvas, Line &line, LineSegment &segment, float x, float y)
	{
		segment.image->draw(canvas, x + segment.x_position, y + line.ascender - segment.ascender);
	}

	void SpanLayoutImpl::draw_layout_text(const CanvasPtr &canvas, Line &line, LineSegment &segment, float x, float y)
	{
		std::string segment_text = text.substr(segment.start, segment.end - segment.start);

		int length = (int)segment_text.length();
		int s1 = clamp((int)sel_start - (int)segment.start, 0, length);
		int s2 = clamp((int)sel_end - (int)segment.start, 0, length);

		if (s1 != s2)
		{
			float xx = x + segment.x_position;
			float xx0 = xx + segment.font->measure_text(canvas, segment_text.substr(0, s1)).advance.width;
			float xx1 = xx0 + segment.font->measure_text(canvas, segment_text.substr(s1, s2 - s1)).advance.width;
			float sel_width = segment.font->measure_text(canvas, segment_text.substr(s1, s2 - s1)).advance.width;

			Path::rect(xx0, y + line.ascender - segment.ascender, xx1 - xx0, segment.ascender + segment.descender)->fill(canvas, sel_background);

			if (cursor_visible && cursor_pos >= segment.start && cursor_pos < segment.end)
			{
				float cursor_x = x + segment.x_position + segment.font->measure_text(canvas, text.substr(segment.start, cursor_pos - segment.start)).advance.width;
				float cursor_width = cursor_overwrite_mode ? segment.font->measure_text(canvas, text.substr(cursor_pos, 1)).advance.width : 1;
				Path::rect(cursor_x, y + line.ascender - segment.ascender, cursor_width, segment.ascender + segment.descender)->fill(canvas, cursor_color);
			}

			if (s1 > 0)
			{
				if (is_ellipsis_draw)
					segment.font->draw_text(canvas, xx, y + line.ascender, segment.font->clipped_text(canvas, ellipsis_content_rect.size(), segment_text.substr(0, s1)), segment.color);
				else
					segment.font->draw_text(canvas, xx, y + line.ascender, segment_text.substr(0, s1), segment.color);
			}
			if (is_ellipsis_draw)
				segment.font->draw_text(canvas, xx0, y + line.ascender, segment.font->clipped_text(canvas, ellipsis_content_rect.size(), segment_text.substr(s1, s2 - s1)), sel_foreground);
			else
				segment.font->draw_text(canvas, xx0, y + line.ascender, segment_text.substr(s1, s2 - s1), sel_foreground);
			xx += sel_width;
			if (s2 < length)
			{
				if (is_ellipsis_draw)
					segment.font->draw_text(canvas, xx1, y + line.ascender, segment.font->clipped_text(canvas, ellipsis_content_rect.size(), segment_text.substr(s2)), segment.color);
				else
					segment.font->draw_text(canvas, xx1, y + line.ascender, segment_text.substr(s2), segment.color);
			}
		}
		else
		{
			if (cursor_visible && cursor_pos >= segment.start && cursor_pos < segment.end)
			{
				float cursor_x = x + segment.x_position + segment.font->measure_text(canvas, text.substr(segment.start, cursor_pos - segment.start)).advance.width;
				float cursor_width = cursor_overwrite_mode ? segment.font->measure_text(canvas, text.substr(cursor_pos, 1)).advance.width : 1;
				Path::rect(cursor_x, y + line.ascender - segment.ascender, cursor_width, segment.ascender + segment.descender)->fill(canvas, cursor_color);
			}

			if (is_ellipsis_draw)
				segment.font->draw_text(canvas, x + segment.x_position, y + line.ascender, segment.font->clipped_text(canvas, ellipsis_content_rect.size(), segment_text), segment.color);
			else
				segment.font->draw_text(canvas, x + segment.x_position, y + line.ascender, segment_text, segment.color);
		}
	}

	SpanLayout::HitTestResult SpanLayoutImpl::hit_test(const CanvasPtr &canvas, const Pointf &pos)
	{
		SpanLayout::HitTestResult result;

		if (lines.empty())
		{
			result.type = SpanLayout::HitTestResult::no_objects_available;
			return result;
		}

		float x = position.x;
		float y = position.y;

		// Check if we are outside to the top
		if (pos.y < y)
		{
			result.type = SpanLayout::HitTestResult::outside_top;
			result.object_id = lines[0].segments[0].id;
			result.offset = 0;
			return result;
		}

		for (auto & elem : lines)
		{
			Line &line = elem;

			// Check if we found current line
			if (pos.y >= y && pos.y <= y + line.height)
			{
				for (std::vector<LineSegment>::size_type segment_index = 0; segment_index < line.segments.size(); segment_index++)
				{
					LineSegment &segment = line.segments[segment_index];

					// Check if we are outside to the left
					if (segment_index == 0 && pos.x < x)
					{
						result.type = SpanLayout::HitTestResult::outside_left;
						result.object_id = segment.id;
						result.offset = segment.start;
						return result;
					}

					// Check if we are inside a segment
					if (pos.x >= x + segment.x_position && pos.x <= x + segment.x_position + segment.width)
					{
						std::string segment_text = text.substr(segment.start, segment.end - segment.start);
						Pointf hit_point(pos.x - x - segment.x_position, 0);
						int offset = segment.start + segment.font->character_index(canvas, segment_text, hit_point);
						if (offset == -1)
							offset = segment_text.size();

						result.type = SpanLayout::HitTestResult::inside;
						result.object_id = segment.id;
						result.offset = offset;
						return result;
					}

					// Check if we are outside to the right
					if (segment_index == line.segments.size() - 1 && pos.x > x + segment.x_position + segment.width)
					{
						result.type = SpanLayout::HitTestResult::outside_right;
						result.object_id = segment.id;
						result.offset = segment.end;
						return result;
					}
				}
			}

			y += line.height;
		}

		// We are outside to the bottom
		const Line &last_line = lines[lines.size() - 1];
		const LineSegment &last_segment = last_line.segments[last_line.segments.size() - 1];

		result.type = SpanLayout::HitTestResult::outside_bottom;
		result.object_id = last_segment.id;
		result.offset = last_segment.end;
		return result;
	}

	Rectf SpanLayoutImpl::rect() const
	{
		float x = position.x;
		float y = position.y;

		const float max_value = 0x7000000;
		Rectf rect(max_value, max_value, -max_value, -max_value);

		for (auto & elem : lines)
		{
			const Line &line = elem;
			for (std::vector<LineSegment>::size_type segment_index = 0; segment_index < line.segments.size(); segment_index++)
			{
				const LineSegment &segment = line.segments[segment_index];
				Rectf area(Pointf(x + segment.x_position, y), Sizef(segment.width, line.height));

				rect.left = min(rect.left, area.left);
				rect.right = max(rect.right, area.right);
				rect.top = min(rect.top, area.top);
				rect.bottom = max(rect.bottom, area.bottom);
			}
			y += line.height;
		}
		if (rect.left > rect.right)
			rect.left = rect.right = position.x;

		if (rect.top > rect.bottom)
			rect.top = rect.bottom = position.y;

		return rect;
	}

	void SpanLayoutImpl::add_text(const std::string &more_text, const FontPtr &font, const Colorf &color, int id)
	{
		SpanObject object;
		object.type = object_text;
		object.start = text.length();
		object.end = object.start + more_text.length();
		object.font = font;
		object.color = color;
		object.id = id;
		objects.push_back(object);
		text += more_text;
	}

	void SpanLayoutImpl::add_image(const ImagePtr &image, float baseline_offset, int id)
	{
		SpanObject object;
		object.type = object_image;
		object.image = image;
		object.baseline_offset = baseline_offset;
		object.id = id;
		object.start = text.length();
		object.end = object.start + 1;
		objects.push_back(object);
		text += "*";
	}

	void SpanLayoutImpl::add_component(std::shared_ptr<SpanComponent> component, float baseline_offset, int id)
	{
		SpanObject object;
		object.type = object_component;
		object.component = component;
		object.baseline_offset = baseline_offset;
		object.id = id;
		object.start = text.length();
		object.end = object.start + 1;
		objects.push_back(object);
		text += "*";
	}

	void SpanLayoutImpl::layout(const CanvasPtr &canvas, float max_width)
	{
		layout_lines(canvas, max_width);

		switch (alignment)
		{
		case SpanAlign::right: align_right(max_width); break;
		case SpanAlign::center: align_center(max_width); break;
		case SpanAlign::justify: align_justify(max_width); break;
		case SpanAlign::left:
		default: break;
		}
	}

	SpanLayoutImpl::TextSizeResult SpanLayoutImpl::find_text_size(const CanvasPtr &canvas, const TextBlock &block, unsigned int object_index)
	{
		FontPtr font = objects[object_index].font;
		if (layout_cache.object_index != object_index)
		{
			layout_cache.object_index = object_index;
			layout_cache.metrics = font->font_metrics(canvas);
		}

		TextSizeResult result;
		result.start = block.start;
		int pos = block.start;
		float x_position = 0;
		while (pos != block.end)
		{
			int end = min(objects[object_index].end, block.end);
			std::string subtext = text.substr(pos, end - pos);

			float text_width = font->measure_text(canvas, subtext).advance.width;

			result.width += text_width;
			result.height = max(result.height, layout_cache.metrics.height() + layout_cache.metrics.external_leading());
			result.ascender = max(result.ascender, layout_cache.metrics.ascent());
			result.descender = max(result.descender, layout_cache.metrics.descent());

			LineSegment segment;
			segment.type = object_text;
			segment.start = pos;
			segment.end = end;
			segment.font = objects[object_index].font;
			segment.color = objects[object_index].color;
			segment.id = objects[object_index].id;
			segment.x_position = x_position;
			segment.width = text_width;
			segment.ascender = layout_cache.metrics.ascent();
			segment.descender = layout_cache.metrics.descent();
			x_position += text_width;
			result.segments.push_back(segment);

			pos = end;
			if (pos == objects[object_index].end)
			{
				object_index++;
				result.objects_traversed++;

				if (object_index < objects.size())
				{
					layout_cache.object_index = object_index;
					font = objects[object_index].font;
					layout_cache.metrics = font->font_metrics(canvas);
				}
			}
		}
		result.end = pos;
		return result;
	}

	std::vector<SpanLayoutImpl::TextBlock> SpanLayoutImpl::find_text_blocks()
	{
		std::vector<TextBlock> blocks;
		std::vector<SpanObject>::iterator block_object_it;

		// Find first object that is not text:
		for (block_object_it = objects.begin(); block_object_it != objects.end() && (*block_object_it).type == object_text; ++block_object_it);

		std::string::size_type pos = 0;
		while (pos < text.size())
		{
			// Find end of text block:
			std::string::size_type end_pos;
			switch (text[pos])
			{
			case ' ':
			case '\t':
			case '\n':
				end_pos = text.find_first_not_of(text[pos], pos);
				break;
			default:
				end_pos = text.find_first_of(" \t\n", pos);
				break;
			}

			if (end_pos == std::string::npos)
				end_pos = text.length();

			// If we traversed past an object that is not text:
			if (block_object_it != objects.end() && (*block_object_it).start < end_pos)
			{
				// End text block
				end_pos = (*block_object_it).start;
				if (end_pos > pos)
				{
					TextBlock block;
					block.start = pos;
					block.end = end_pos;
					blocks.push_back(block);
				}

				// Create object block:
				pos = end_pos;
				end_pos = pos + 1;

				TextBlock block;
				block.start = pos;
				block.end = end_pos;
				blocks.push_back(block);

				// Find next object that is not text:
				for (++block_object_it; block_object_it != objects.end() && (*block_object_it).type == object_text; ++block_object_it);
			}
			else
			{
				if (end_pos > pos)
				{
					TextBlock block;
					block.start = pos;
					block.end = end_pos;
					blocks.push_back(block);
				}
			}

			pos = end_pos;
		}

		return blocks;
	}

	void SpanLayoutImpl::set_align(SpanAlign align)
	{
		alignment = align;
	}

	void SpanLayoutImpl::layout_lines(const CanvasPtr &canvas, float max_width)
	{
		lines.clear();
		if (objects.empty())
			return;

		layout_cache.metrics = FontMetrics();
		layout_cache.object_index = -1;

		CurrentLine current_line;
		std::vector<TextBlock> blocks = find_text_blocks();
		for (std::vector<TextBlock>::size_type block_index = 0; block_index < blocks.size(); block_index++)
		{
			if (objects[current_line.object_index].type == object_text)
				layout_text(canvas, blocks, block_index, current_line, max_width);
			else
				layout_block(current_line, max_width, blocks, block_index);
		}
		next_line(current_line);
	}

	void SpanLayoutImpl::layout_block(CurrentLine &current_line, float max_width, std::vector<TextBlock> &blocks, std::vector<TextBlock>::size_type block_index)
	{
		if (objects[current_line.object_index].float_type == float_none)
			layout_inline_block(current_line, max_width, blocks, block_index);
		else
			layout_float_block(current_line, max_width);

		current_line.object_index++;
	}

	void SpanLayoutImpl::layout_inline_block(CurrentLine &current_line, float max_width, std::vector<TextBlock> &blocks, std::vector<TextBlock>::size_type block_index)
	{
		Sizef size;
		LineSegment segment;
		if (objects[current_line.object_index].type == object_image)
		{
			size = objects[current_line.object_index].image->size();
			segment.type = object_image;
			segment.image = objects[current_line.object_index].image;
		}
		else if (objects[current_line.object_index].type == object_component)
		{
			size = objects[current_line.object_index].component->size();
			segment.type = object_component;
			segment.component = objects[current_line.object_index].component.get();
		}

		if (current_line.x_position + size.width > max_width)
			next_line(current_line);

		segment.x_position = current_line.x_position;
		segment.width = size.width;
		segment.start = blocks[block_index].start;
		segment.end = blocks[block_index].end;
		segment.id = objects[current_line.object_index].id;
		segment.ascender = size.height - objects[current_line.object_index].baseline_offset;
		current_line.cur_line.segments.push_back(segment);
		current_line.cur_line.height = max(current_line.cur_line.height, size.height + objects[current_line.object_index].baseline_offset);
		current_line.cur_line.ascender = max(current_line.cur_line.ascender, segment.ascender);
		current_line.x_position += size.width;
	}

	void SpanLayoutImpl::layout_float_block(CurrentLine &current_line, float max_width)
	{
		FloatBox floatbox;
		floatbox.type = objects[current_line.object_index].type;
		floatbox.image = objects[current_line.object_index].image;
		floatbox.component = objects[current_line.object_index].component.get();
		floatbox.id = objects[current_line.object_index].id;
		if (objects[current_line.object_index].type == object_image)
			floatbox.rect = Rectf(Pointf(0, current_line.y_position), Sizef(floatbox.image->size()));
		else if (objects[current_line.object_index].type == object_component)
			floatbox.rect = Rectf(Pointf(0, current_line.y_position), Sizef(floatbox.component->size()));

		if (objects[current_line.object_index].float_type == float_left)
			floats_left.push_back(float_box_left(floatbox, max_width));
		else
			floats_right.push_back(float_box_right(floatbox, max_width));

		reflow_line(current_line, max_width);
	}

	void SpanLayoutImpl::reflow_line(CurrentLine &step, float max_width)
	{
	}

	SpanLayoutImpl::FloatBox SpanLayoutImpl::float_box_left(FloatBox box, float max_width)
	{
		return float_box_any(box, max_width, floats_left);
	}

	SpanLayoutImpl::FloatBox SpanLayoutImpl::float_box_right(FloatBox box, float max_width)
	{
		return float_box_any(box, max_width, floats_right);
	}

	SpanLayoutImpl::FloatBox SpanLayoutImpl::float_box_any(FloatBox box, float max_width, const std::vector<FloatBox> &floats1)
	{
		bool restart;
		do
		{
			restart = false;
			for (auto & elem : floats1)
			{
				float top = max(elem.rect.top, box.rect.top);
				float bottom = min(elem.rect.bottom, box.rect.bottom);
				if (bottom > top && box.rect.left < elem.rect.right)
				{
					Sizef s = box.rect.size();
					box.rect.left = elem.rect.left;
					box.rect.right = box.rect.left + s.width;

					if (!box_fits_on_line(box, max_width))
					{
						box.rect.left = 0;
						box.rect.right = s.width;
						box.rect.top = elem.rect.bottom;
						box.rect.bottom = box.rect.top + s.height;
						restart = true;
						break;
					}
				}
			}
		} while (restart);
		return box;
	}

	bool SpanLayoutImpl::box_fits_on_line(const FloatBox &box, float max_width)
	{
		for (auto & elem : floats_right)
		{
			float top = max(elem.rect.top, box.rect.top);
			float bottom = min(elem.rect.bottom, box.rect.bottom);
			if (bottom > top)
			{
				if (box.rect.right + elem.rect.right > max_width)
					return false;
			}
		}
		return true;
	}

	void SpanLayoutImpl::layout_text(const CanvasPtr &canvas, std::vector<TextBlock> blocks, std::vector<TextBlock>::size_type block_index, CurrentLine &current_line, float max_width)
	{
		TextSizeResult text_size_result = find_text_size(canvas, blocks[block_index], current_line.object_index);
		current_line.object_index += text_size_result.objects_traversed;

		current_line.cur_line.width = current_line.x_position;

		if (is_newline(blocks[block_index]))
		{
			current_line.cur_line.height = max(current_line.cur_line.height, text_size_result.height);
			current_line.cur_line.ascender = max(current_line.cur_line.ascender, text_size_result.ascender);
			next_line(current_line);
		}
		else
		{
			if (!fits_on_line(current_line.x_position, text_size_result, max_width) && !is_whitespace(blocks[block_index]))
			{
				if (larger_than_line(text_size_result, max_width))
				{
					// force line breaks to make it fit
					force_place_line_segments(current_line, text_size_result, max_width);
				}
				else
				{
					next_line(current_line);
					place_line_segments(current_line, text_size_result);
				}
			}
			else
			{
				place_line_segments(current_line, text_size_result);
			}
		}
	}

	void SpanLayoutImpl::next_line(CurrentLine &current_line)
	{
		current_line.cur_line.width = current_line.x_position;
		for (auto it = current_line.cur_line.segments.rbegin(); it != current_line.cur_line.segments.rend(); ++it)
		{
			LineSegment &segment = *it;
			if (segment.type == object_text)
			{
				std::string s = text.substr(segment.start, segment.end - segment.start);
				if (s.find_first_not_of(" \t\r\n") != std::string::npos)
				{
					current_line.cur_line.width = segment.x_position + segment.width;
					break;
				}
				else
				{
					// We remove the width so that get_rect() reports the correct sizes
					segment.width = 0;
				}
			}
			else
			{
				current_line.cur_line.width = segment.x_position + segment.width;
				break;
			}
		}

		float height = current_line.cur_line.height;
		lines.push_back(current_line.cur_line);
		current_line.cur_line = Line();
		current_line.x_position = 0;
		current_line.y_position += height;
	}

	void SpanLayoutImpl::place_line_segments(CurrentLine &current_line, TextSizeResult &text_size_result)
	{
		for (auto segment : text_size_result.segments)
		{

			segment.x_position += current_line.x_position;
			current_line.cur_line.segments.push_back(segment);
		}
		current_line.x_position += text_size_result.width;
		current_line.cur_line.height = max(current_line.cur_line.height, text_size_result.height);
		current_line.cur_line.ascender = max(current_line.cur_line.ascender, text_size_result.ascender);
	}

	void SpanLayoutImpl::force_place_line_segments(CurrentLine &current_line, TextSizeResult &text_size_result, float max_width)
	{
		if (current_line.x_position != 0)
			next_line(current_line);

		// to do: do this properly - for now we just place the entire block on one line
		place_line_segments(current_line, text_size_result);
	}

	bool SpanLayoutImpl::is_newline(const TextBlock &block)
	{
		return block.start != block.end && text[block.start] == '\n';
	}

	bool SpanLayoutImpl::is_whitespace(const TextBlock &block)
	{
		return block.start != block.end && text[block.start] == ' ';
	}

	bool SpanLayoutImpl::fits_on_line(float x_position, const TextSizeResult &text_size_result, float max_width)
	{
		return x_position + text_size_result.width <= max_width;
	}

	bool SpanLayoutImpl::larger_than_line(const TextSizeResult &text_size_result, float max_width)
	{
		return text_size_result.width > max_width;
	}

	void SpanLayoutImpl::align_right(float max_width)
	{
		for (auto & elem : lines)
		{
			Line &line = elem;
			float offset = max_width - line.width;
			if (offset < 0) offset = 0;

			for (auto & _segment_index : line.segments)
			{
				LineSegment &segment = _segment_index;
				segment.x_position += offset;
			}
		}
	}

	void SpanLayoutImpl::align_center(float max_width)
	{
		for (auto & elem : lines)
		{
			Line &line = elem;
			float offset = (max_width - line.width) / 2;
			if (offset < 0) offset = 0;

			for (auto & _segment_index : line.segments)
			{
				LineSegment &segment = _segment_index;
				segment.x_position += offset;
			}
		}
	}

	void SpanLayoutImpl::align_justify(float max_width)
	{
		// Note, we do not justify the last line
		for (std::vector<Line>::size_type line_index = 0; line_index + 1 < lines.size(); line_index++)
		{
			Line &line = lines[line_index];
			float offset = max_width - line.width;
			if (offset < 0) offset = 0;

			if (line.segments.size() <= 1)	// Do not justify line if only one word exists
				continue;

			for (std::vector<LineSegment>::size_type segment_index = 0; segment_index < line.segments.size(); segment_index++)
			{
				LineSegment &segment = line.segments[segment_index];
				segment.x_position += (offset * segment_index) / (line.segments.size() - 1);
			}
		}
	}

	Sizef SpanLayoutImpl::find_preferred_size(const CanvasPtr &canvas)
	{
		layout_lines(canvas, 0x70000000); // Feed it with a very long length so it ends up on one line
		return rect().size();
	}

	void SpanLayoutImpl::set_selection_range(std::string::size_type start, std::string::size_type end)
	{
		sel_start = start;
		sel_end = end;
		if (sel_end < sel_start)
			sel_end = sel_start;
	}

	void SpanLayoutImpl::set_selection_colors(const Colorf &foreground, const Colorf &background)
	{
		sel_foreground = foreground;
		sel_background = background;
	}

	std::string SpanLayoutImpl::combined_text() const
	{
		return text;
	}

	void SpanLayoutImpl::set_component_geometry()
	{
		float x = position.x;
		float y = position.y;
		for (auto & elem : lines)
		{
			for (size_t j = 0; j < elem.segments.size(); j++)
			{
				if (elem.segments[j].type == object_component)
				{
					Pointf pos(x + elem.segments[j].x_position, y + elem.ascender - elem.segments[j].ascender);
					Sizef size = elem.segments[j].component->size();
					Rectf rect(pos, size);
					elem.segments[j].component->set_geometry(rect);
				}
			}
			y += elem.height;
		}
	}

	float SpanLayoutImpl::first_baseline_offset()
	{
		if (!lines.empty())
		{
			return lines.front().ascender;
		}
		else
		{
			return 0;
		}
	}

	float SpanLayoutImpl::last_baseline_offset()
	{
		if (!lines.empty())
		{
			float y = 0;
			for (size_t i = 0; i + 1 < lines.size(); i++)
				y += lines[i].height;
			return y + lines.back().ascender;
		}
		else
		{
			return 0;
		}
	}
}
