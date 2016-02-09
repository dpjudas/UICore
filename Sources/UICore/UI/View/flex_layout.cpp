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
#include "flex_layout.h"
#include <algorithm>
#include <cmath>

namespace uicore
{
	void FlexLayout::create_row_items(const CanvasPtr &canvas, View *view)
	{
		items.clear();

		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (!subview->is_static_position_and_visible())
				continue;

			const auto &item_style = subview->style_cascade();

			FlexLayoutItem item;
			item.view = subview.get();

			// Definite sizes:

			item.definite_main_size = item_style.computed_value("width").is_length();
			item.definite_cross_size = item_style.computed_value("height").is_length();
			item.definite_min_main_size = item_style.computed_value("min-width").is_length();
			item.definite_max_main_size = item_style.computed_value("max-width").is_length();
			item.definite_min_cross_size = item_style.computed_value("min-height").is_length();
			item.definite_max_cross_size = item_style.computed_value("max-height").is_length();

			if (item.definite_main_size)
				item.main_size = item_style.computed_value("width").number();
			if (item.definite_cross_size)
				item.cross_size = item_style.computed_value("height").number();
			if (item.definite_min_main_size)
				item.min_main_size = item_style.computed_value("min-width").number();
			if (item.definite_max_main_size)
				item.max_main_size = item_style.computed_value("max-width").number();
			if (item.definite_min_cross_size)
				item.min_cross_size = item_style.computed_value("min-height").number();
			if (item.definite_max_cross_size)
				item.max_cross_size = item_style.computed_value("max-height").number();

			// Main axis auto min:

			if (item_style.computed_value("min-width").is_keyword("auto"))
			{
				float min_content_size = 0.0f; // item.view->min_content_width(); // shrink-to-fit in CSS 2.1
				if (item.definite_main_size)
				{
					float specified_size = item.main_size;
					if (item.definite_max_main_size)
						specified_size = std::min(specified_size, item.max_main_size);
					item.min_main_size = std::min(specified_size, min_content_size);
				}
				else if (item.definite_cross_size /*&& item.view->intrinsic_aspect_ratio() != 0.0f*/)
				{
					float clamped_cross_size = item.cross_size;

					if (item.definite_min_cross_size)
						clamped_cross_size = std::max(clamped_cross_size, item.min_cross_size);

					if (item.definite_max_cross_size)
						clamped_cross_size = std::min(clamped_cross_size, item.max_cross_size);

					float aspect = 1.0f; // item.view->intrinsic_aspect_ratio();
					float transfered_size = clamped_cross_size / aspect;
					item.min_main_size = std::min(transfered_size, min_content_size);
				}
				else
				{
					item.min_main_size = min_content_size;
				}
			}

			// Non-content sizes:

			item.main_noncontent_start += subview->style_cascade().computed_value("margin-left").number();
			item.main_noncontent_start += subview->style_cascade().computed_value("border-left-width").number();
			item.main_noncontent_start += subview->style_cascade().computed_value("padding-left").number();
			item.main_noncontent_end += subview->style_cascade().computed_value("padding-right").number();
			item.main_noncontent_end += subview->style_cascade().computed_value("border-right-width").number();
			item.main_noncontent_end += subview->style_cascade().computed_value("margin-right").number();

			item.cross_noncontent_start += subview->style_cascade().computed_value("margin-top").number();
			item.cross_noncontent_start += subview->style_cascade().computed_value("border-top-width").number();
			item.cross_noncontent_start += subview->style_cascade().computed_value("padding-top").number();
			item.cross_noncontent_end += subview->style_cascade().computed_value("padding-bottom").number();
			item.cross_noncontent_end += subview->style_cascade().computed_value("border-bottom-width").number();
			item.cross_noncontent_end += subview->style_cascade().computed_value("margin-bottom").number();

			items.push_back(item);
		}
	}

	void FlexLayout::create_column_items(const CanvasPtr &canvas, View *view)
	{
		items.clear();

		for (const std::shared_ptr<View> &subview : view->subviews())
		{
			if (!subview->is_static_position_and_visible())
				continue;

			const auto &item_style = subview->style_cascade();

			FlexLayoutItem item;
			item.view = subview.get();

			// Definite sizes:

			item.definite_main_size = item_style.computed_value("height").is_length();
			item.definite_cross_size = item_style.computed_value("width").is_length();
			item.definite_min_main_size = item_style.computed_value("min-height").is_length();
			item.definite_max_main_size = item_style.computed_value("max-height").is_length();
			item.definite_min_cross_size = item_style.computed_value("min-width").is_length();
			item.definite_max_cross_size = item_style.computed_value("max-width").is_length();

			if (item.definite_main_size)
				item.main_size = item_style.computed_value("height").number();
			if (item.definite_cross_size)
				item.cross_size = item_style.computed_value("width").number();
			if (item.definite_min_main_size)
				item.min_main_size = item_style.computed_value("min-height").number();
			if (item.definite_max_main_size)
				item.max_main_size = item_style.computed_value("max-height").number();
			if (item.definite_min_cross_size)
				item.min_cross_size = item_style.computed_value("min-width").number();
			if (item.definite_max_cross_size)
				item.max_cross_size = item_style.computed_value("max-width").number();

			// Main axis auto min:

			if (item_style.computed_value("min-height").is_keyword("auto"))
			{
				float min_content_size = 0.0f; // item.view->preferred_height(canvas, item.view->min_content_width()); // shrink-to-fit in CSS 2.1
				if (item.definite_main_size)
				{
					float specified_size = item.main_size;
					if (item.definite_max_main_size)
						specified_size = std::min(specified_size, item.max_main_size);
					item.min_main_size = std::min(specified_size, min_content_size);
				}
				else if (item.definite_cross_size /*&& item.view->intrinsic_aspect_ratio() != 0.0f*/)
				{
					float clamped_cross_size = item.cross_size;

					if (item.definite_min_cross_size)
						clamped_cross_size = std::max(clamped_cross_size, item.min_cross_size);

					if (item.definite_max_cross_size)
						clamped_cross_size = std::min(clamped_cross_size, item.max_cross_size);

					float aspect = 1.0f; // item.view->intrinsic_aspect_ratio();
					float transfered_size = clamped_cross_size * aspect;
					item.min_main_size = std::min(transfered_size, min_content_size);
				}
				else
				{
					item.min_main_size = min_content_size;
				}
			}

			// Non-content sizes:

			item.main_noncontent_start += subview->style_cascade().computed_value("margin-top").number();
			item.main_noncontent_start += subview->style_cascade().computed_value("border-top-width").number();
			item.main_noncontent_start += subview->style_cascade().computed_value("padding-top").number();
			item.main_noncontent_end += subview->style_cascade().computed_value("padding-bottom").number();
			item.main_noncontent_end += subview->style_cascade().computed_value("border-bottom-width").number();
			item.main_noncontent_end += subview->style_cascade().computed_value("margin-bottom").number();

			item.cross_noncontent_start += subview->style_cascade().computed_value("margin-left").number();
			item.cross_noncontent_start += subview->style_cascade().computed_value("border-left-width").number();
			item.cross_noncontent_start += subview->style_cascade().computed_value("padding-left").number();
			item.cross_noncontent_end += subview->style_cascade().computed_value("padding-right").number();
			item.cross_noncontent_end += subview->style_cascade().computed_value("border-right-width").number();
			item.cross_noncontent_end += subview->style_cascade().computed_value("margin-right").number();

			items.push_back(item);
		}
	}

	void FlexLayout::calculate_row_basis(const CanvasPtr &canvas)
	{
		total_flex_grow = 0.0f;
		total_flex_shrink = 0.0f;
		total_flex_basis = 0.0f;
		total_main_noncontent = 0.0f;

		for (auto &item : items)
		{
			if (!item.definite_main_size)
			{
				if (item.view->style_cascade().computed_value("flex-basis").is_length())
					item.flex_basis = item.view->style_cascade().computed_value("flex-basis").number();
				else if (item.definite_main_size)
					item.flex_basis = item.main_size;
				else
					item.flex_basis = item.view->preferred_width(canvas);

				if (item.definite_min_main_size)
					item.flex_basis = std::max(item.flex_basis, item.min_main_size);

				if (item.definite_max_main_size)
					item.flex_basis = std::min(item.flex_basis, item.max_main_size);

				item.flex_grow = item.view->style_cascade().computed_value("flex-grow").number();
				item.flex_shrink = item.view->style_cascade().computed_value("flex-shrink").number();

				total_flex_grow += item.flex_grow;
				total_flex_shrink += item.flex_shrink;
				total_flex_basis += item.flex_basis;
				total_main_noncontent += item.main_noncontent_start + item.main_noncontent_start;
			}
		}
	}

	void FlexLayout::calculate_column_basis(const CanvasPtr &canvas)
	{
		total_flex_grow = 0.0f;
		total_flex_shrink = 0.0f;
		total_flex_basis = 0.0f;
		total_main_noncontent = 0.0f;

		for (auto &item : items)
		{
			if (!item.definite_main_size)
			{
				if (item.view->style_cascade().computed_value("flex-basis").is_length())
					item.flex_basis = item.view->style_cascade().computed_value("flex-basis").number();
				else if (item.definite_main_size)
					item.flex_basis = item.main_size;
				else
					item.flex_basis = item.view->preferred_height(canvas, item.cross_size);

				if (item.definite_min_main_size)
					item.flex_basis = std::max(item.flex_basis, item.min_main_size);

				if (item.definite_max_main_size)
					item.flex_basis = std::min(item.flex_basis, item.max_main_size);

				item.flex_grow = item.view->style_cascade().computed_value("flex-grow").number();
				item.flex_shrink = item.view->style_cascade().computed_value("flex-shrink").number();

				total_flex_grow += item.flex_grow;
				total_flex_shrink += item.flex_shrink;
				total_flex_basis += item.flex_basis;
				total_main_noncontent += item.main_noncontent_start + item.main_noncontent_end;
			}
		}
	}

	void FlexLayout::calculate_single_row_flex_main_size(const CanvasPtr &canvas, View *view)
	{
		calculate_single_flex_main_size(canvas, view->geometry().content_width);
	}

	void FlexLayout::calculate_single_column_flex_main_size(const CanvasPtr &canvas, View *view)
	{
		calculate_single_flex_main_size(canvas, view->geometry().content_height);
	}

	void FlexLayout::calculate_single_flex_main_size(const CanvasPtr &canvas, float content_size)
	{
		float free_space = content_size - total_main_noncontent - total_flex_basis;

		for (auto &item : items)
		{
			float size = item.flex_basis;

			if (free_space < 0.0f && total_flex_shrink != 0.0f)
				size += item.flex_shrink * free_space / total_flex_shrink;
			else if (free_space > 0.0f && total_flex_grow != 0.0f)
				size += item.flex_grow * free_space / total_flex_grow;

			item.flex_main_size = size;
		}
	}
}
