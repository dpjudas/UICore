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
#include "UICore/UI/StandardViews/slider_view.h"
#include "UICore/Display/2D/path.h"
#include "UICore/Display/System/timer.h"
#include "UICore/Display/2D/brush.h"
#include "UICore/UI/Events/pointer_event.h"
#include "UICore/UI/Events/focus_change_event.h"
#include "UICore/UI/Events/activation_change_event.h"
#include <algorithm>
#include "slider_view_impl.h"

namespace uicore
{
	SliderView::SliderView() : impl(std::make_shared<SliderBaseViewImpl>())
	{
		impl->slider = this;

		impl->track = std::make_shared<View>();
		impl->thumb = std::make_shared<View>();

		//add_child(impl->track);
		//add_child(impl->thumb);
	
		connect(sig_pointer_press(), [this](PointerEvent *e) { impl->on_pointer_track_press(*e); });
		connect(sig_pointer_release(), [this](PointerEvent *e) { impl->on_pointer_track_release(*e); });

		connect(sig_pointer_press(), [this](PointerEvent *e) { impl->on_pointer_thumb_press(*e); });
		connect(sig_pointer_release(), [this](PointerEvent *e) { impl->on_pointer_thumb_release(*e); });

		connect(sig_pointer_enter(), [&](PointerEvent *e) { impl->_state_hot = true;  impl->update_state(); });
		connect(sig_pointer_leave(), [&](PointerEvent *e) { impl->_state_hot = false;  impl->update_state(); });

		connect(impl->thumb->sig_pointer_move(), [this](PointerEvent *e) { impl->on_pointer_move(*e); });

		connect(sig_focus_gained(), [this](FocusChangeEvent *e) { impl->on_focus_gained(*e); });
		connect(sig_focus_lost(), [this](FocusChangeEvent *e) { impl->on_focus_lost(*e); });
		connect(sig_activated(), [this](ActivationChangeEvent *e) { impl->on_activated(*e); });
		connect(sig_deactivated(), [this](ActivationChangeEvent *e) { impl->on_deactivated(*e); });

		impl->scroll_timer->func_expired() = uicore::bind_member(impl.get(), &SliderBaseViewImpl::scroll_timer_expired);

		set_vertical();
	}

	bool SliderView::disabled() const
	{
		return impl->_state_disabled;
	}

	void SliderView::set_disabled()
	{
		if (!impl->_state_disabled)
		{
			impl->_state_disabled = true;
			impl->update_state();
			impl->mouse_down_mode = SliderBaseViewImpl::mouse_down_none;
			impl->scroll_timer->stop();
		}
	}
	void SliderView::set_enabled()
	{
		if (impl->_state_disabled)
		{
			impl->_state_disabled = false;
			impl->update_state();
		}
	}

	std::shared_ptr<View> SliderView::track() const
	{
		return impl->track;
	}

	std::shared_ptr<View> SliderView::thumb() const
	{
		return impl->thumb;
	}

	bool SliderView::vertical() const
	{
		return impl->_vertical;
	}

	bool SliderView::horizontal() const
	{
		return !impl->_vertical;
	}

	void SliderView::set_vertical()
	{
		impl->_vertical = true;
		//impl->thumb->style()->set("position: absolute; top: 0px; left: auto");
		set_needs_layout();
	}

	void SliderView::set_horizontal()
	{
		impl->_vertical = false;
		//impl->thumb->style()->set("position: absolute; left: 0px; top: auto");
		set_needs_layout();
	}

	int SliderView::max_position() const
	{
		return impl->_max_position;
	}
	int SliderView::position() const
	{
		return impl->_position;
	}
	int SliderView::min_position() const
	{
		return impl->_min_position;
	}

	void SliderView::set_max_position(int value)
	{
		int new_min = std::min(impl->_min_position, value);
		int new_max = value;
		int new_pos = std::max(std::min(value, new_max), new_min);
		impl->update_pos(this, new_pos, new_min, new_max);

	}
	void SliderView::set_position(int value)
	{
		int new_pos = std::max(std::min(value, impl->_max_position), impl->_min_position);
		impl->update_pos(this, new_pos, impl->_min_position, impl->_max_position);
	}

	void SliderView::set_min_position(int value)
	{
		int new_min = value;
		int new_max = std::max(impl->_max_position, value);
		int new_pos = std::max(std::min(value, new_max), new_min);
		impl->update_pos(this, new_pos, new_min, new_max);
	}

	void SliderView::set_tick_count(int tick_count)
	{
		impl->_tick_count = tick_count;
		set_needs_layout();
	}

	void SliderView::set_page_step(int page_step)
	{
		impl->_page_step = page_step;
		set_needs_layout();
	}

	void SliderView::set_lock_to_ticks(bool lock)
	{
		impl->_lock_to_ticks = lock;
	}

	/*
	void SliderView::layout_children(const std::shared_ptr<Canvas> &canvas)
	{
		View::layout_children(canvas);

		auto track_geometry = impl->track->geometry();

		float track_length = vertical() ? track_geometry.content_box().height() : track_geometry.content_box().width();
		float thumb_length = vertical() ? impl->thumb->preferred_height(canvas, track_geometry.content_box().width()) : impl->thumb->preferred_width(canvas);

		float t = (float) (impl->_position - impl->_min_position) / (float) (impl->_max_position - impl->_min_position);
		float thumb_pos = t * (track_length - thumb_length);

		if (vertical())
		{
			impl->thumb->style()->set("top: %1px;", thumb_pos);
		}
		else
		{
			impl->thumb->style()->set("left: %1px;", thumb_pos);
		}
	}
	*/

	Signal<void()> &SliderView::sig_value_changed()
	{
		return impl->sig_value_changed;
	}
}
