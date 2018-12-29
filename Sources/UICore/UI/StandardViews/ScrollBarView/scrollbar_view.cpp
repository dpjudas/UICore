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
**    Mark Page
*/

#include "UICore/precomp.h"
#include "UICore/UI/StandardViews/scrollbar_view.h"
#include "UICore/UI/Style/style_property_parser.h"
#include "UICore/Display/2D/path.h"
#include "UICore/Display/System/timer.h"
#include "UICore/Display/2D/brush.h"
#include "UICore/UI/Events/pointer_event.h"
#include "UICore/UI/Events/focus_change_event.h"
#include "UICore/UI/Events/activation_change_event.h"
#include <algorithm>
#include "scrollbar_view_impl.h"

namespace uicore
{
#if 0
	ScrollBarView::ScrollBarView(bool render_button_arrows) : impl(std::make_shared<ScrollBarViewImpl>())
	{
		impl->scrollbar = this;

		impl->button_decrement = std::make_shared<ScrollBarButtonBaseView>(render_button_arrows);
		impl->button_increment = std::make_shared<ScrollBarButtonBaseView>(render_button_arrows);
		impl->track = std::make_shared<View>();
		impl->thumb = std::make_shared<View>();
		impl->thumb_grip = std::make_shared<View>();
		auto spacer1 = std::make_shared<View>();
		auto spacer2 = std::make_shared<View>();

		add_child(impl->button_decrement);
		add_child(impl->track);
		add_child(impl->button_increment);
		add_child(impl->thumb);

		impl->thumb->add_child(spacer1);
		impl->thumb->add_child(impl->thumb_grip);
		impl->thumb->add_child(spacer2);

		impl->button_decrement->style()->set("flex: 0 0 auto");
		impl->button_increment->style()->set("flex: 0 0 auto");
		impl->track->style()->set("flex: 1 1 auto");
		spacer1->style()->set("flex: 1 1 auto");
		spacer2->style()->set("flex: 1 1 auto");

		impl->button_decrement->style()->set("width: 17px; height: 17px");
		impl->button_increment->style()->set("width: 17px; height: 17px");

		slots.connect(impl->track->sig_pointer_press(), [this](PointerEvent *e) { impl->on_pointer_track_press(*e); });
		slots.connect(impl->track->sig_pointer_release(), [this](PointerEvent *e) { impl->on_pointer_track_release(*e); });

		slots.connect(impl->thumb->sig_pointer_press(), [this](PointerEvent *e) { impl->on_pointer_thumb_press(*e); });
		slots.connect(impl->thumb->sig_pointer_release(), [this](PointerEvent *e) { impl->on_pointer_thumb_release(*e); });

		slots.connect(impl->button_decrement->sig_pointer_press(), [this](PointerEvent *e) { impl->on_pointer_decrement_press(*e); });
		slots.connect(impl->button_decrement->sig_pointer_release(), [this](PointerEvent *e) { impl->on_pointer_decrement_release(*e); });
		slots.connect(impl->button_increment->sig_pointer_press(), [this](PointerEvent *e) { impl->on_pointer_increment_press(*e); });
		slots.connect(impl->button_increment->sig_pointer_release(), [this](PointerEvent *e) { impl->on_pointer_increment_release(*e); });

		slots.connect(impl->thumb->sig_pointer_move(), [this](PointerEvent *e) { impl->on_pointer_move(*e); });

		slots.connect(sig_focus_gained(), [this](FocusChangeEvent *e) { impl->on_focus_gained(*e); });
		slots.connect(sig_focus_lost(), [this](FocusChangeEvent *e) { impl->on_focus_lost(*e); });
		slots.connect(sig_activated(), [this](ActivationChangeEvent *e) { impl->on_activated(*e); });
		slots.connect(sig_deactivated(), [this](ActivationChangeEvent *e) { impl->on_deactivated(*e); });

		slots.connect(impl->track->sig_pointer_enter(), [&](PointerEvent *e) { impl->_state_track_hot = true; impl->update_track_state(); });
		slots.connect(impl->track->sig_pointer_leave(), [&](PointerEvent *e) { impl->_state_track_hot = false; impl->update_track_state(); });
		slots.connect(impl->thumb->sig_pointer_enter(), [&](PointerEvent *e) { impl->_state_thumb_hot = true; impl->update_thumb_state(); });
		slots.connect(impl->thumb->sig_pointer_leave(), [&](PointerEvent *e) { impl->_state_thumb_hot = false; impl->update_thumb_state(); });
		slots.connect(impl->thumb_grip->sig_pointer_enter(), [&](PointerEvent *e) { impl->_state_thumb_hot = true; impl->update_thumb_state(); });
		slots.connect(impl->thumb_grip->sig_pointer_leave(), [&](PointerEvent *e) { impl->_state_thumb_hot = false; impl->update_thumb_state(); });
		slots.connect(spacer1->sig_pointer_enter(), [&](PointerEvent *e) { impl->_state_thumb_hot = true; impl->update_thumb_state(); });
		slots.connect(spacer1->sig_pointer_leave(), [&](PointerEvent *e) { impl->_state_thumb_hot = false; impl->update_thumb_state(); });
		slots.connect(spacer2->sig_pointer_enter(), [&](PointerEvent *e) {impl->_state_thumb_hot = true; impl->update_thumb_state(); });
		slots.connect(spacer2->sig_pointer_leave(), [&](PointerEvent *e) {impl->_state_thumb_hot = false; impl->update_thumb_state(); });
		slots.connect(impl->button_decrement->sig_pointer_enter(), [&](PointerEvent *e) { impl->_state_decrement_hot = true; impl->update_decrement_state(); });
		slots.connect(impl->button_decrement->sig_pointer_leave(), [&](PointerEvent *e) { impl->_state_decrement_hot = false; impl->update_decrement_state(); });
		slots.connect(impl->button_increment->sig_pointer_enter(), [&](PointerEvent *e) { impl->_state_increment_hot = true; impl->update_increment_state(); });
		slots.connect(impl->button_increment->sig_pointer_leave(), [&](PointerEvent *e) { impl->_state_increment_hot = false;  impl->update_increment_state(); });

		impl->scroll_timer->func_expired() = uicore::bind_member(impl.get(), &ScrollBarViewImpl::scroll_timer_expired);

		set_vertical();
	}

	std::shared_ptr<View> ScrollBarView::button_decrement() const
	{
		return impl->button_decrement;
	}

	std::shared_ptr<View> ScrollBarView::button_increment() const
	{
		return impl->button_increment;
	}

	std::shared_ptr<View> ScrollBarView::track() const
	{
		return impl->track;
	}

	std::shared_ptr<View> ScrollBarView::thumb() const
	{
		return impl->thumb;
	}

	std::shared_ptr<View> ScrollBarView::thumb_grip() const
	{
		return impl->thumb_grip;
	}

	bool ScrollBarView::vertical() const
	{
		return impl->_vertical;
	}

	bool ScrollBarView::horizontal() const
	{
		return !vertical();
	}

	void ScrollBarView::set_vertical()
	{
		if (impl->_vertical)
			return;

		impl->_vertical = true;
		impl->button_decrement->style()->set("flex-direction: column");
		impl->button_increment->style()->set("flex-direction: column");
		impl->track->style()->set("flex-direction: column");
		impl->thumb->style()->set("flex-direction: column");
		impl->thumb_grip->style()->set("flex-direction: column");
		impl->button_decrement->set_direction(ScrollBarButtonDirection::up);
		impl->button_increment->set_direction(ScrollBarButtonDirection::down);
	}

	void ScrollBarView::set_horizontal()
	{
		if (!impl->_vertical)
			return;

		impl->_vertical = false;
		impl->button_decrement->style()->set("flex-direction: row");
		impl->button_increment->style()->set("flex-direction: row");
		impl->track->style()->set("flex-direction: row");
		impl->thumb->style()->set("flex-direction: row");
		impl->thumb_grip->style()->set("flex-direction: row");
		impl->button_decrement->set_direction(ScrollBarButtonDirection::left);
		impl->button_increment->set_direction(ScrollBarButtonDirection::right);
	}

	double ScrollBarView::line_step() const
	{
		return impl->line_step;
	}

	double ScrollBarView::page_step() const
	{
		return impl->page_step;
	}

	void ScrollBarView::set_line_step(double value)
	{
		impl->line_step = value;
	}

	void ScrollBarView::set_page_step(double value)
	{
		impl->page_step = value;
	}

	double ScrollBarView::min_position() const
	{
		return impl->min_pos;
	}

	double ScrollBarView::max_position() const
	{
		return impl->max_pos;
	}

	double ScrollBarView::position() const
	{
		return impl->pos;
	}

	bool ScrollBarView::disabled() const
	{
		return impl->_state_disabled;
	}

	void ScrollBarView::set_disabled()
	{
		if (!impl->_state_disabled)
		{
			impl->_state_disabled = true;
			impl->update_thumb_state();
			impl->update_track_state();
			impl->update_increment_state();
			impl->update_decrement_state();

			impl->mouse_down_mode = ScrollBarViewImpl::mouse_down_none;
			impl->scroll_timer->stop();

		}
	}
	void ScrollBarView::set_enabled()
	{
		if (impl->_state_disabled)
		{
			impl->_state_disabled = false;
			impl->update_thumb_state();
			impl->update_track_state();
			impl->update_increment_state();
			impl->update_decrement_state();
		}
	}

	void ScrollBarView::set_min_position(double value)
	{
		double new_min = value;
		double new_max = std::max(impl->max_pos, value);
		double new_pos = std::max(std::min(impl->pos, new_max), new_min);
		impl->update_pos(this, new_pos, new_min, new_max);
	}

	void ScrollBarView::set_max_position(double value)
	{
		double new_min = std::min(impl->min_pos, value);
		double new_max = value;
		double new_pos = std::max(std::min(impl->pos, new_max), new_min);
		impl->update_pos(this, new_pos, new_min, new_max);
	}

	void ScrollBarView::set_range(double min_value, double max_value)
	{
		double new_min = min_value;
		double new_max = max_value;
		double new_pos = std::max(std::min(impl->pos, new_max), new_min);
		impl->update_pos(this, new_pos, new_min, new_max);
	}

	void ScrollBarView::set_position(double value)
	{
		double new_pos = std::max(std::min(value, impl->max_pos), impl->min_pos);
		impl->update_pos(this, new_pos, impl->min_pos, impl->max_pos);
	}

	void ScrollBarView::layout_children(const std::shared_ptr<Canvas> &canvas)
	{
		// Place buttons and track:
		if (vertical())
		{
			float width = geometry().content_width;
			float height = geometry().content_height;
			float incr_height = impl->button_increment->preferred_margin_height(canvas, width);
			float decr_height = impl->button_decrement->preferred_margin_height(canvas, width);
			float track_height = std::max(height - incr_height - decr_height, 0.0f);
			impl->button_decrement->set_margin_geometry(Rectf::xywh(0.0f, 0.0f, width, decr_height));
			impl->button_increment->set_margin_geometry(Rectf::xywh(0.0f, height - incr_height, width, incr_height));
			impl->track->set_margin_geometry(Rectf::xywh(0.0f, incr_height, width, std::max(height - incr_height - decr_height, 0.0f)));
		}
		else
		{
			float width = geometry().content_width;
			float height = geometry().content_height;
			float incr_width = impl->button_increment->preferred_margin_width(canvas);
			float decr_width = impl->button_decrement->preferred_margin_width(canvas);
			float track_width = std::max(width - incr_width - decr_width, 0.0f);
			impl->button_decrement->set_margin_geometry(Rectf::xywh(0.0f, 0.0f, decr_width, height));
			impl->button_increment->set_margin_geometry(Rectf::xywh(width - incr_width, 0.0f, incr_width, height));
			impl->track->set_margin_geometry(Rectf::xywh(incr_width, 0.0f, std::max(width - incr_width - decr_width, 0.0f), height));
		}

		// Place thumb within the track geometry:
		auto track = impl->track->geometry();
		if (impl->min_pos == impl->max_pos || impl->page_step == 0.0)
		{
			impl->thumb->set_margin_geometry(Rectf::xywh(track.content_x, track.content_y, track.content_width, track.content_height));
		}
		else
		{
			double thumb_pos = impl->thumb_pos();
			double thumb_length = impl->thumb_length();

			if (vertical())
			{
				impl->thumb->set_margin_geometry(Rectf::xywh(track.content_x, track.content_y + (float)thumb_pos, track.content_width, (float)thumb_length));
			}
			else
			{
				impl->thumb->set_margin_geometry(Rectf::xywh(track.content_x + (float)thumb_pos, track.content_y, (float)thumb_length, track.content_height));
			}
		}

		// Update children layout (to do: maybe View should do this automatically on its own after this call finished?)
		impl->button_increment->layout_children(canvas);
		impl->button_decrement->layout_children(canvas);
		impl->track->layout_children(canvas);
	}

	float ScrollBarView::calculate_preferred_width(const std::shared_ptr<Canvas> &canvas)
	{
		float button_incr_size = impl->button_increment->preferred_margin_width(canvas);
		float button_decr_size = impl->button_decrement->preferred_margin_width(canvas);
		float track_size = impl->track->preferred_margin_width(canvas);
		if (impl->_vertical)
		{
			return std::max({ button_incr_size, button_decr_size, track_size });
		}
		else
		{
			return button_incr_size + button_decr_size + track_size;
		}
	}

	float ScrollBarView::calculate_preferred_height(const std::shared_ptr<Canvas> &canvas, float width)
	{
		float button_incr_size = impl->button_increment->preferred_margin_height(canvas, width);
		float button_decr_size = impl->button_decrement->preferred_margin_height(canvas, width);
		float track_size = impl->track->preferred_margin_height(canvas, width);
		if (impl->_vertical)
		{
			return button_incr_size + button_decr_size + track_size;
		}
		else
		{
			return std::max({ button_incr_size, button_decr_size, track_size });
		}
	}

	float ScrollBarView::calculate_first_baseline_offset(const std::shared_ptr<Canvas> &canvas, float width)
	{
		return 0.0f;
	}

	float ScrollBarView::calculate_last_baseline_offset(const std::shared_ptr<Canvas> &canvas, float width)
	{
		return 0.0f;
	}

	Signal<void()> &ScrollBarView::sig_scroll()
	{
		return impl->sig_scroll;
	}
#endif
}
