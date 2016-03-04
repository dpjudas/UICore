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
#include "UICore/UI/StandardViews/scroll_view.h"
#include "UICore/UI/StandardViews/scrollbar_view.h"
#include "UICore/UI/Events/pointer_event.h"
#include <algorithm>

namespace uicore
{
	class ScrollBaseViewContentContainer : public View
	{
	public:
		void layout_children(const CanvasPtr &canvas) override
		{
			for (auto &view : children())
			{
				// To do: maybe we need a mode to specify if the X axis is locked or infinite
				float width = geometry().content_width; //view->preferred_width(canvas);
				float height = view->preferred_height(canvas, width);
				ViewGeometry geometry = ViewGeometry::from_content_box(style_cascade(), Rectf(0.0f, 0.0f, width, height));
				geometry.content_x = 0.0f;
				geometry.content_y = 0.0f;
				view->set_geometry(geometry);

				view->layout_children(canvas); // Maybe this should be handled in View?
			}
		}
	};

	class ScrollBaseViewImpl
	{
	public:
		ScrollBaseView *view = nullptr;
		std::shared_ptr<ScrollBarBaseView> scroll_x = std::make_shared<ScrollBarBaseView>();
		std::shared_ptr<ScrollBarBaseView> scroll_y = std::make_shared<ScrollBarBaseView>();
		std::shared_ptr<ScrollBaseViewContentContainer> content_container = std::make_shared<ScrollBaseViewContentContainer>();
		std::shared_ptr<View> content = std::make_shared<View>();
		ContentOverflow overflow_x = ContentOverflow::hidden;
		ContentOverflow overflow_y = ContentOverflow::automatic;
		Pointf content_offset;
	};

	ScrollBaseView::ScrollBaseView() : impl(new ScrollBaseViewImpl())
	{
		impl->view = this;
		
		impl->scroll_x->set_hidden();
		impl->scroll_y->set_hidden();
		impl->scroll_x->set_horizontal();
		impl->scroll_y->set_vertical();

		impl->content_container->style()->set("flex: 1 1 auto");

		impl->content_container->set_content_clipped(true);
		impl->content_container->add_child(impl->content);

		add_child(impl->content_container);
		add_child(impl->scroll_x);
		add_child(impl->scroll_y);

		slots.connect(impl->scroll_x->sig_scroll(), [this]() {
			Pointf pos = content_offset();
			pos.x = (float)impl->scroll_x->position();
			set_content_offset(pos);
		});

		slots.connect(impl->scroll_y->sig_scroll(), [this]() {
			Pointf pos = content_offset();
			pos.y = (float)impl->scroll_y->position();
			set_content_offset(pos);
		});
	}

	ScrollBaseView::~ScrollBaseView()
	{
	}

	std::shared_ptr<View> ScrollBaseView::content_view() const
	{
		return impl->content;
	}

	std::shared_ptr<ScrollBarBaseView> ScrollBaseView::scrollbar_x_view() const
	{
		return impl->scroll_x;
	}

	std::shared_ptr<ScrollBarBaseView> ScrollBaseView::scrollbar_y_view() const
	{
		return impl->scroll_y;
	}
	
	ContentOverflow ScrollBaseView::overflow_x() const
	{
		return impl->overflow_x;
	}
	
	ContentOverflow ScrollBaseView::overflow_y() const
	{
		return impl->overflow_y;
	}
	
	void ScrollBaseView::set_overflow_x(ContentOverflow value)
	{
		if (impl->overflow_x == value)
			return;
		
		impl->overflow_x = value;
		set_needs_layout();
	}
	
	void ScrollBaseView::set_overflow_y(ContentOverflow value)
	{
		if (impl->overflow_y == value)
			return;
		
		impl->overflow_y = value;
		set_needs_layout();
	}
	
	void ScrollBaseView::set_overflow(ContentOverflow value_x, ContentOverflow value_y)
	{
		set_overflow_x(value_x);
		set_overflow_y(value_y);
	}
	
	Pointf ScrollBaseView::content_offset() const
	{
		return impl->content_offset;
	}
	
	void ScrollBaseView::set_content_offset(const Pointf &offset, bool animated)
	{
		if (impl->content_offset == offset)
			return;
		
		impl->content_offset = offset;
		impl->content->set_view_transform(Mat4f::translate(-offset.x, -offset.y, 0.0f));
	}
	
	void ScrollBaseView::layout_children(const CanvasPtr &canvas)
	{
		bool x_scroll_needed = false;
		bool y_scroll_needed = false;
		float content_width = 0.0f;
		float content_height = 0.0f;
		float y_scroll_width = 0.0f;
		float x_scroll_height = 0.0f;
		
		float width = geometry().content_box().width();
		float height = geometry().content_box().height();
		
		if (impl->overflow_y != ContentOverflow::hidden)
		{
			content_height = impl->content_container->preferred_height(canvas, width);
			y_scroll_needed = impl->overflow_y == ContentOverflow::scroll || content_height > height;
			if (y_scroll_needed)
				y_scroll_width = ViewGeometry::from_content_box(impl->scroll_y->style_cascade(), Rectf(0.0f, 0.0f, impl->scroll_y->preferred_width(canvas), 0.0f)).margin_box().width();
		}
		
		if (impl->overflow_x != ContentOverflow::hidden)
		{
			content_width = impl->content_container->preferred_width(canvas);
			x_scroll_needed = impl->overflow_x == ContentOverflow::scroll || content_width > width;
			if (x_scroll_needed)
				x_scroll_height = ViewGeometry::from_content_box(impl->scroll_x->style_cascade(), Rectf(0.0f, 0.0f, 0.0f, impl->scroll_x->preferred_height(canvas, width))).margin_box().height();
		}
		
		float content_view_width = width - y_scroll_width;
		float content_view_height = height - x_scroll_height;
		
		if (x_scroll_needed)
		{
			impl->scroll_x->set_max_position(std::max(content_width - content_view_width, 0.0f));
			impl->scroll_x->set_page_step(content_view_width);
		}
		
		if (y_scroll_needed)
		{
			impl->scroll_y->set_max_position(std::max(content_height - content_view_height, 0.0f));
			impl->scroll_y->set_page_step(content_view_height);
		}
		
		impl->scroll_x->set_hidden(!x_scroll_needed);
		impl->scroll_y->set_hidden(!y_scroll_needed);
		
		impl->scroll_x->set_geometry(ViewGeometry::from_margin_box(impl->scroll_x->style_cascade(), Rectf(0.0f, content_view_height, width - y_scroll_width, height)));
		impl->scroll_y->set_geometry(ViewGeometry::from_margin_box(impl->scroll_y->style_cascade(), Rectf(content_view_width, 0.0f, width, height - x_scroll_height)));
		
		impl->content_container->set_geometry(ViewGeometry::from_margin_box(impl->content_container->style_cascade(), Rectf(0.0f, 0.0f, content_view_width, content_view_height)));

		impl->scroll_x->layout_children(canvas);
		impl->scroll_y->layout_children(canvas);
		impl->content_container->layout_children(canvas);
	}
	
	float ScrollBaseView::calculate_preferred_width(const CanvasPtr &canvas)
	{
		float width = impl->content_container->preferred_width(canvas);
		if (impl->overflow_x == ContentOverflow::scroll)
			width += impl->scroll_x->preferred_width(canvas);
		return width;
	}
	
	float ScrollBaseView::calculate_preferred_height(const CanvasPtr &canvas, float width)
	{
		float height = impl->content_container->preferred_height(canvas, width);
		if (impl->overflow_y == ContentOverflow::scroll)
			height += impl->scroll_x->preferred_height(canvas, width);
		return height;
	}
	
	float ScrollBaseView::calculate_first_baseline_offset(const CanvasPtr &canvas, float width)
	{
		return impl->content_container->first_baseline_offset(canvas, width);
	}
	
	float ScrollBaseView::calculate_last_baseline_offset(const CanvasPtr &canvas, float width)
	{
		return impl->content_container->last_baseline_offset(canvas, width);
	}
}
