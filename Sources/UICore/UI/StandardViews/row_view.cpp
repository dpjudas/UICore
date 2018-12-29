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
#include "UICore/UI/StandardViews/row_view.h"
#include <cmath>
#include <algorithm>

namespace uicore
{
	class RowViewImpl
	{
	public:
		std::vector<std::shared_ptr<View>> views;
		bool needs_layout = true;
	};

	RowView::RowView() : impl(new RowViewImpl())
	{
	}

	RowView::~RowView()
	{
	}

	void RowView::add_child(std::shared_ptr<View> view, const char *style)
	{
		view->set_parent(this);
		view->set_theme(UITheme::get()->create_view_theme(style));
		impl->views.push_back(view);
	}

	void RowView::remove_child(std::shared_ptr<View> view)
	{
		auto it = std::find(impl->views.begin(), impl->views.end(), view);
		if (it != impl->views.end())
		{
			view->set_parent(nullptr);
			impl->views.erase(it);
		}
	}

	float RowView::preferred_width(const std::shared_ptr<Canvas> &canvas)
	{
		float w = 0.0f;
		for (const auto &view : impl->views)
		{
			w = std::max(view->preferred_width(canvas), w);
		}
		return w;
	}

	float RowView::preferred_height(const std::shared_ptr<Canvas> &canvas, float width)
	{
		float h = 0.0f;
		for (const auto &view : impl->views)
		{
			h += view->preferred_height(canvas, width);
		}
		return h;
	}

	float RowView::first_baseline_offset(const std::shared_ptr<Canvas> &canvas, float width)
	{
		if (!impl->views.empty())
			return impl->views.front()->first_baseline_offset(canvas, width);
		else
			return 0.0f;
	}

	float RowView::last_baseline_offset(const std::shared_ptr<Canvas> &canvas, float width)
	{
		if (!impl->views.empty())
		{
			float h = 0.0f;
			for (const auto &view : impl->views)
			{
				if (view != impl->views.back())
					h += view->preferred_height(canvas, width);
			}
			return h + impl->views.back()->last_baseline_offset(canvas, width);
		}
		else
		{
			return 0.0f;
		}
	}

	bool RowView::needs_layout() const
	{
		return impl->needs_layout;
	}

	void RowView::set_needs_layout()
	{
		impl->needs_layout = true;
		View::set_needs_layout();
	}

	std::shared_ptr<View> RowView::find_view_at(const Pointf &pos) const
	{
		for (const auto &view : impl->views)
		{
			if (view->geometry().border_box().contains(pos))
			{
				return view->find_view_at(pos - view->geometry().content_pos());
			}
		}
		return const_cast<RowView*>(this)->shared_from_this();
	}

	void RowView::render_content(const std::shared_ptr<Canvas> &canvas)
	{
		if (impl->needs_layout)
		{
			float width = geometry().content_width;
			float y = 0.0f;
			for (const auto &view : impl->views)
			{
				ViewGeometry childpos;
				childpos.content_y = y;
				childpos.content_width = width;
				childpos.content_height = view->preferred_height(canvas, width);
				view->set_geometry(childpos);

				y += childpos.content_height;
			}

			impl->needs_layout = false;
		}

		for (const auto &view : impl->views)
		{
			view->render(canvas);
		}
	}
}
