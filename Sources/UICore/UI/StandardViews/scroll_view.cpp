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
**    Artem Khomenko
*/

#include "UICore/precomp.h"
#include "UICore/UI/StandardViews/scroll_view.h"
#include "UICore/UI/StandardViews/scrollbar_view.h"
#include "UICore/UI/Events/pointer_event.h"
#include <algorithm>

namespace uicore
{
#if 0
	class ScrollBaseViewContentContainer : public View
	{
	public:
		ScrollBaseViewContentContainer()
		{
			set_content_clipped(true);
			set_content_view(std::make_shared<ColumnView>());
		}
		
		void layout_children(const std::shared_ptr<Canvas> &canvas) override
		{
			if (!content)
				return;

			const auto &item_style = content->style_cascade();
			
			float noncontent_left = item_style.computed_value("margin-left").number();
			noncontent_left += item_style.computed_value("border-left-width").number();
			noncontent_left += item_style.computed_value("padding-left").number();
			
			float noncontent_right = item_style.computed_value("padding-right").number();
			noncontent_right += item_style.computed_value("border-right-width").number();
			noncontent_right += item_style.computed_value("margin-right").number();
			
			float noncontent_top = item_style.computed_value("margin-top").number();
			noncontent_top += item_style.computed_value("border-top-width").number();
			noncontent_top += item_style.computed_value("padding-top").number();
			
			float noncontent_bottom = item_style.computed_value("padding-bottom").number();
			noncontent_bottom += item_style.computed_value("border-bottom-width").number();
			noncontent_bottom += item_style.computed_value("margin-bottom").number();
			
			float width = 0.0f;
			if (item_style.computed_value("width").is_length())
				width = item_style.computed_value("width").number();
			else
				width = infinite_width ? content->preferred_width(canvas) : geometry().content_width - noncontent_left - noncontent_right;
			
			if (item_style.computed_value("min-width").is_length())
				width = std::max(width, item_style.computed_value("min-width").number());
					
			if (item_style.computed_value("max-width").is_length())
				width = std::min(width, item_style.computed_value("max-width").number());
				

			float height = 0.0f;
			if (item_style.computed_value("height").is_length())
				height = item_style.computed_value("height").number();
			else
				height = infinite_height ? content->preferred_height(canvas, width) : geometry().content_height - noncontent_top - noncontent_bottom;
			
			if (item_style.computed_value("min-height").is_length())
				height = std::max(height, item_style.computed_value("min-height").number());
				
			if (item_style.computed_value("max-height").is_length())
				height = std::min(height, item_style.computed_value("max-height").number());
				
			content->set_geometry(ViewGeometry::from_content_box(item_style, Rectf(noncontent_left, noncontent_top, width, height)));
				
			content->layout_children(canvas);
		}
		
		float calculate_preferred_width(const std::shared_ptr<Canvas> &canvas) override
		{
			if (!content)
				return 0.0f;
			
			const auto &item_style = content->style_cascade();
			
			float noncontent_left = item_style.computed_value("margin-left").number();
			noncontent_left += item_style.computed_value("border-left-width").number();
			noncontent_left += item_style.computed_value("padding-left").number();
			
			float noncontent_right = item_style.computed_value("padding-right").number();
			noncontent_right += item_style.computed_value("border-right-width").number();
			noncontent_right += item_style.computed_value("margin-right").number();
			
			float width = 0.0f;
			if (item_style.computed_value("width").is_length())
				width = item_style.computed_value("width").number();
			else
				width = content->preferred_width(canvas);
			
			if (item_style.computed_value("min-width").is_length())
				width = std::max(width, item_style.computed_value("min-width").number());
				
			if (item_style.computed_value("max-width").is_length())
				width = std::min(width, item_style.computed_value("max-width").number());
				
			return width + noncontent_left + noncontent_right;
		}
		
		float calculate_preferred_height(const std::shared_ptr<Canvas> &canvas, float container_width) override
		{
			if (!content)
				return 0.0f;
			
			const auto &item_style = content->style_cascade();
			
			float noncontent_left = item_style.computed_value("margin-left").number();
			noncontent_left += item_style.computed_value("border-left-width").number();
			noncontent_left += item_style.computed_value("padding-left").number();
			
			float noncontent_right = item_style.computed_value("padding-right").number();
			noncontent_right += item_style.computed_value("border-right-width").number();
			noncontent_right += item_style.computed_value("margin-right").number();
			
			float noncontent_top = item_style.computed_value("margin-top").number();
			noncontent_top += item_style.computed_value("border-top-width").number();
			noncontent_top += item_style.computed_value("padding-top").number();
			
			float noncontent_bottom = item_style.computed_value("padding-bottom").number();
			noncontent_bottom += item_style.computed_value("border-bottom-width").number();
			noncontent_bottom += item_style.computed_value("margin-bottom").number();
			
			float width = 0.0f;
			if (item_style.computed_value("width").is_length())
				width = item_style.computed_value("width").number();
			else
				width = infinite_width ? content->preferred_width(canvas) : container_width - noncontent_left - noncontent_right;
					
			if (item_style.computed_value("min-width").is_length())
				width = std::max(width, item_style.computed_value("min-width").number());
						
			if (item_style.computed_value("max-width").is_length())
				width = std::min(width, item_style.computed_value("max-width").number());
			
			float height = 0.0f;
			if (item_style.computed_value("height").is_length())
				height = item_style.computed_value("height").number();
			else
				height = content->preferred_height(canvas, width);
					
			if (item_style.computed_value("min-height").is_length())
				height = std::max(height, item_style.computed_value("min-height").number());
						
			if (item_style.computed_value("max-height").is_length())
				height = std::min(height, item_style.computed_value("max-height").number());
			
			return height;
		}
		
		float calculate_first_baseline_offset(const std::shared_ptr<Canvas> &canvas, float container_width) override
		{
			if (!content)
				return 0.0f;
			
			const auto &item_style = content->style_cascade();
			
			float noncontent_left = item_style.computed_value("margin-left").number();
			noncontent_left += item_style.computed_value("border-left-width").number();
			noncontent_left += item_style.computed_value("padding-left").number();
			
			float noncontent_right = item_style.computed_value("padding-right").number();
			noncontent_right += item_style.computed_value("border-right-width").number();
			noncontent_right += item_style.computed_value("margin-right").number();
			
			float width = 0.0f;
			if (item_style.computed_value("width").is_length())
				width = item_style.computed_value("width").number();
			else
				width = infinite_width ? content->preferred_width(canvas) : container_width - noncontent_left - noncontent_right;
			
			if (item_style.computed_value("min-width").is_length())
				width = std::max(width, item_style.computed_value("min-width").number());
				
			if (item_style.computed_value("max-width").is_length())
				width = std::min(width, item_style.computed_value("max-width").number());
			
			return content->first_baseline_offset(canvas, width);
		}
		
		float calculate_last_baseline_offset(const std::shared_ptr<Canvas> &canvas, float container_width) override
		{
			if (!content)
				return 0.0f;
			
			const auto &item_style = content->style_cascade();
			
			float noncontent_left = item_style.computed_value("margin-left").number();
			noncontent_left += item_style.computed_value("border-left-width").number();
			noncontent_left += item_style.computed_value("padding-left").number();
			
			float noncontent_right = item_style.computed_value("padding-right").number();
			noncontent_right += item_style.computed_value("border-right-width").number();
			noncontent_right += item_style.computed_value("margin-right").number();
			
			float width = 0.0f;
			if (item_style.computed_value("width").is_length())
				width = item_style.computed_value("width").number();
			else
				width = infinite_width ? content->preferred_width(canvas) : container_width - noncontent_left - noncontent_right;
			
			if (item_style.computed_value("min-width").is_length())
				width = std::max(width, item_style.computed_value("min-width").number());
				
			if (item_style.computed_value("max-width").is_length())
				width = std::min(width, item_style.computed_value("max-width").number());
					
			return content->last_baseline_offset(canvas, width);
		}
		
		std::shared_ptr<View> content_view() const
		{
			return content;
		}
		
		void set_content_view(std::shared_ptr<View> view)
		{
			if (view == content)
				return;

			if (content)
				content->remove_from_parent();
				
			content = view;
				
			if (content)
			{
				add_child(content);
				content->set_view_transform(Mat4f::translate(-_content_offset.x, -_content_offset.y, 0.0f));
			}
		}
		
		Pointf content_offset() const
		{
			return _content_offset;
		}
		
		void set_content_offset(const Pointf &offset, bool animated)
		{
			if (_content_offset == offset)
				return;
			
			_content_offset = offset;
			content->set_view_transform(Mat4f::translate(-offset.x, -offset.y, 0.0f));
		}
		
		bool infinite_content_width() const
		{
			return infinite_width;
		}
		
		bool infinite_content_height() const
		{
			return infinite_height;
		}
		
		void set_infinite_content_width(bool enable)
		{
			if (infinite_width != enable)
			{
				infinite_width = enable;
				set_needs_layout();
			}
		}
		
		void set_infinite_content_height(bool enable)
		{
			if (infinite_height != enable)
			{
				infinite_height = enable;
				set_needs_layout();
			}
		}
		
	private:
		std::shared_ptr<View> content;
		Pointf _content_offset;
		bool infinite_width = false;
		bool infinite_height = true;
	};

	class ScrollBaseViewImpl
	{
	public:
		ScrollBaseView *view = nullptr;
		std::shared_ptr<ScrollBarBaseView> scroll_x = std::make_shared<ScrollBarBaseView>();
		std::shared_ptr<ScrollBarBaseView> scroll_y = std::make_shared<ScrollBarBaseView>();
		std::shared_ptr<ScrollBaseViewContentContainer> content_container = std::make_shared<ScrollBaseViewContentContainer>();
		ContentOverflow overflow_x = ContentOverflow::hidden;
		ContentOverflow overflow_y = ContentOverflow::automatic;

		// The quantity of line_steps to scroll when mouse wheel event is occurred.
		float num_steps_on_mouse_wheel = 3.0f;
	};

	ScrollBaseView::ScrollBaseView() : impl(new ScrollBaseViewImpl())
	{
		impl->view = this;
		
		impl->scroll_x->set_hidden();
		impl->scroll_y->set_hidden();
		impl->scroll_x->set_horizontal();
		impl->scroll_y->set_vertical();

		add_child(impl->content_container);
		add_child(impl->scroll_x);
		add_child(impl->scroll_y);

		slots.connect(impl->scroll_x->sig_scroll(), [this]()
		{
			Pointf pos = content_offset();
			pos.x = (float)impl->scroll_x->position();
			set_content_offset(pos);
		});

		slots.connect(impl->scroll_y->sig_scroll(), [this]()
		{
			Pointf pos = content_offset();
			pos.y = (float)impl->scroll_y->position();
			set_content_offset(pos);
		});

		// Process mouse wheel events. When Shift pressed, scrolls horizontally, otherwise vertically.
		slots.connect(sig_pointer_press(), [this](PointerEvent *e)
		{
			if (e->button() == PointerButton::wheel_down)
			{
				auto scroll_bar = e->shift_down() ? impl->scroll_x : impl->scroll_y;
				if (!scroll_bar->hidden())
				{
					double old_pos = scroll_bar->position();
					scroll_bar->set_position(old_pos + impl->num_steps_on_mouse_wheel * scroll_bar->line_step());

					// If position changed, stop propagation this event to parent, if not - give a chance to underlying views.
					if (old_pos != scroll_bar->position())
						e->stop_propagation();
				}
			}
			else if (e->button() == PointerButton::wheel_up)
			{
				auto scroll_bar = e->shift_down() ? impl->scroll_x : impl->scroll_y;
				if (!scroll_bar->hidden())
				{
					double old_pos = scroll_bar->position();
					scroll_bar->set_position(scroll_bar->position() - impl->num_steps_on_mouse_wheel * scroll_bar->line_step());

					// If position changed, stop propagation this event to parent, if not - give a chance to underlying views.
					if (old_pos != scroll_bar->position())
						e->stop_propagation();
				}
			}
		});
	}

	ScrollBaseView::~ScrollBaseView()
	{
	}

	std::shared_ptr<View> ScrollBaseView::content_view() const
	{
		return impl->content_container->content_view();
	}
	
	void ScrollBaseView::set_content_view(std::shared_ptr<View> view)
	{
		impl->content_container->set_content_view(view);
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
	
	bool ScrollBaseView::infinite_content_width() const
	{
		return impl->content_container->infinite_content_width();
	}
	
	bool ScrollBaseView::infinite_content_height() const
	{
		return impl->content_container->infinite_content_height();
	}
	
	void ScrollBaseView::set_infinite_content_width(bool enable)
	{
		impl->content_container->set_infinite_content_width(enable);
		set_needs_layout();
	}
	
	void ScrollBaseView::set_infinite_content_height(bool enable)
	{
		impl->content_container->set_infinite_content_height(enable);
		set_needs_layout();
	}
	
	Pointf ScrollBaseView::content_offset() const
	{
		return impl->content_container->content_offset();
	}
	
	void ScrollBaseView::set_content_offset(const Pointf &offset, bool animated)
	{
		impl->content_container->set_content_offset(offset, animated);
	}
	
	void ScrollBaseView::layout_children(const std::shared_ptr<Canvas> &canvas)
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
		
		// Exclude from the content_box of the space occupied by scrollbars.
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

		// Update scroll_bar's thumb pos.
		impl->scroll_x->layout_children(canvas);
		impl->scroll_y->layout_children(canvas);
		impl->content_container->layout_children(canvas);

		// The position can be changed when user has increased size of view (scrolled to top left), update it.
		set_content_offset(Pointf(impl->scroll_x->position(), impl->scroll_y->position()));
	}
	
	float ScrollBaseView::calculate_preferred_width(const std::shared_ptr<Canvas> &canvas)
	{
		float width = impl->content_container->preferred_width(canvas);
		if (impl->overflow_x == ContentOverflow::scroll)
			width += impl->scroll_x->preferred_width(canvas);
		return width;
	}
	
	float ScrollBaseView::calculate_preferred_height(const std::shared_ptr<Canvas> &canvas, float width)
	{
		float height = impl->content_container->preferred_height(canvas, width);
		if (impl->overflow_y == ContentOverflow::scroll)
			height += impl->scroll_x->preferred_height(canvas, width);
		return height;
	}
	
	float ScrollBaseView::calculate_first_baseline_offset(const std::shared_ptr<Canvas> &canvas, float width)
	{
		return impl->content_container->first_baseline_offset(canvas, width);
	}
	
	float ScrollBaseView::calculate_last_baseline_offset(const std::shared_ptr<Canvas> &canvas, float width)
	{
		return impl->content_container->last_baseline_offset(canvas, width);
	}
#endif
}
