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
#include "positioned_layout.h"
#include <algorithm>

namespace uicore
{
	void PositionedLayout::layout_children(const CanvasPtr &canvas, View *view)
	{
		for (const std::shared_ptr<View> &child : view->children())
		{
			if (child->hidden())
			{
				continue;
			}
			else if (child->style_cascade().computed_value("position").is_keyword("absolute"))
			{
				// To do: decide how we determine the containing box used for absolute positioning. For now, use the parent content box.
				layout_from_containing_box(canvas, child.get(), view->geometry().content_box());
			}
			else if (child->style_cascade().computed_value("position").is_keyword("fixed"))
			{
				Rectf offset_initial_containing_box;
				View *current = view->parent();
				if (current)
				{
					Pointf offset(view->geometry().content_x, view->geometry().content_y);
					while (true)
					{
						offset = offset + Pointf(current->geometry().content_x, current->geometry().content_y);
						View *parent = current->parent();
						if (!parent)
						{
							offset_initial_containing_box = current->geometry().content_box();
							offset_initial_containing_box.set_position(offset_initial_containing_box.position() - offset);
							break;
						}
					}
				}
				else
				{
					offset_initial_containing_box = view->geometry().content_box();
				}

				layout_from_containing_box(canvas, child.get(), offset_initial_containing_box);
			}

			layout_children(canvas, child.get());
		}
	}

	ViewGeometry PositionedLayout::get_geometry(const CanvasPtr &canvas, View *view, const Rectf &containing_box)
	{
		bool definite_left = !view->style_cascade().computed_value("left").is_keyword("auto");
		bool definite_right = !view->style_cascade().computed_value("right").is_keyword("auto");
		bool definite_width = !view->style_cascade().computed_value("width").is_keyword("auto");

		float computed_left = resolve_percentage(view->style_cascade().computed_value("left"), containing_box.width());
		float computed_right = resolve_percentage(view->style_cascade().computed_value("right"), containing_box.width());
		float computed_width = resolve_percentage(view->style_cascade().computed_value("width"), containing_box.width());

		float x = 0.0f;
		float width = 0.0f;

		if (definite_left && definite_right)
		{
			x = computed_left;
			width = uicore::max(containing_box.width() - computed_right - x, 0.0f);
		}
		else if (definite_left && definite_width)
		{
			x = computed_left;
			width = computed_width;
		}
		else if (definite_right && definite_width)
		{
			width = computed_width;
			x = containing_box.width() - computed_right - width;
		}
		else if (definite_left)
		{
			x = computed_left;
			width = view->preferred_width(canvas);
		}
		else if (definite_right)
		{
			width = view->preferred_width(canvas);
			x = containing_box.width() - computed_right - width;
		}
		else if (definite_width)
		{
			x = 0.0f;
			width = view->style_cascade().computed_value("width").number();
		}
		else
		{
			x = 0.0f;
			width = view->preferred_width(canvas);
		}

		bool definite_top = !view->style_cascade().computed_value("top").is_keyword("auto");
		bool definite_bottom = !view->style_cascade().computed_value("bottom").is_keyword("auto");
		bool definite_height = !view->style_cascade().computed_value("height").is_keyword("auto");

		float computed_top = resolve_percentage(view->style_cascade().computed_value("top"), containing_box.height());
		float computed_bottom = resolve_percentage(view->style_cascade().computed_value("bottom"), containing_box.height());
		float computed_height = resolve_percentage(view->style_cascade().computed_value("height"), containing_box.height());

		float y = 0.0f;
		float height = 0.0f;

		if (definite_top && definite_bottom)
		{
			y = computed_top;
			height = uicore::max(containing_box.height() - computed_bottom - y, 0.0f);
		}
		else if (definite_top && definite_height)
		{
			y = computed_top;
			height = computed_height;
		}
		else if (definite_bottom && definite_height)
		{
			height = computed_height;
			y = containing_box.height() - computed_bottom - height;
		}
		else if (definite_top)
		{
			y = computed_top;
			height = view->preferred_height(canvas, width);
		}
		else if (definite_bottom)
		{
			height = view->preferred_height(canvas, width);
			y = containing_box.height() - computed_bottom - height;
		}
		else if (definite_height)
		{
			y = 0.0f;
			height = computed_height;
		}
		else
		{
			y = 0.0f;
			height = view->preferred_height(canvas, width);
		}

		return ViewGeometry::from_content_box(view->style_cascade(), Rectf::xywh(x, y, width, height));
	}

	float PositionedLayout::resolve_percentage(StyleGetValue &computed_value, float size)
	{
		if (computed_value.is_percentage())
			return computed_value.number() * size / 100.0f;
		else
			return computed_value.number();
	}

	void PositionedLayout::layout_from_containing_box(const CanvasPtr &canvas, View *view, const Rectf &containing_box)
	{
		view->set_geometry(get_geometry(canvas, view, containing_box));
		view->layout_children(canvas);
	}
}
