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

#include "UICore/UI/View/view.h"

namespace uicore
{
	class FlexLayoutItem
	{
	public:
		View *view = nullptr;

		bool definite_main_size = false;
		bool definite_cross_size = false;

		bool definite_min_main_size = false;
		bool definite_max_main_size = false;

		bool definite_min_cross_size = false;
		bool definite_max_cross_size = false;

		float main_size = 0.0f;
		float cross_size = 0.0f;

		float min_main_size = 0.0f;
		float max_main_size = 1e10f;

		float min_cross_size = 0.0f;
		float max_cross_size = 1e10f;

		float main_noncontent_start = 0.0f;
		float main_noncontent_end = 0.0f;
		float cross_noncontent_start = 0.0f;
		float cross_noncontent_end = 0.0f;

		float flex_basis = 0.0f;
		float flex_grow = 0.0f;
		float flex_shrink = 0.0f;

		float flex_main_size = 0.0f;
		float flex_cross_size = 0.0f;
	};

	class FlexLayout
	{
	public:
		std::vector<FlexLayoutItem> items;

		float total_flex_grow = 0.0f;
		float total_flex_shrink = 0.0f;
		float total_flex_basis = 0.0f;
		float total_main_noncontent = 0.0f;

		void create_row_items(const CanvasPtr &canvas, View *view);
		void create_column_items(const CanvasPtr &canvas, View *view);

		void calculate_row_basis(const CanvasPtr &canvas);
		void calculate_column_basis(const CanvasPtr &canvas);

		void calculate_single_row_flex_main_size(const CanvasPtr &canvas, View *view);
		void calculate_single_column_flex_main_size(const CanvasPtr &canvas, View *view);

		void calculate_single_flex_main_size(const CanvasPtr &canvas, float content_size);
	};
}
