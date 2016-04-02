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
#include "scrollbar_view_impl.h"
#include <algorithm>

namespace uicore
{
	ScrollBarButtonBaseView::ScrollBarButtonBaseView(bool render_button_arrows) : _render_button_arrows(render_button_arrows)
	{
	}

	void ScrollBarButtonBaseView::render_content(const CanvasPtr &canvas)
	{
		if (!_render_button_arrows)
			return;

		Rectf box = geometry().content_box().size();
		box.shrink(4.0f, 6.0f);

		auto path = Path::create();
		path->set_fill_mode(PathFillMode::alternate);
		switch (direction)
		{
		case ScrollBarButtonDirection::left:
			path->move_to(box.right, box.top);
			path->line_to(box.right, box.bottom);
			path->line_to(box.left, box.top + box.height() * 0.5f);
			path->close();
			break;
		case ScrollBarButtonDirection::right:
			path->move_to(box.left, box.top);
			path->line_to(box.left, box.bottom);
			path->line_to(box.right, box.top + box.height() * 0.5f);
			path->close();
			break;
		case ScrollBarButtonDirection::up:
			path->move_to(box.left, box.bottom);
			path->line_to(box.right, box.bottom);
			path->line_to(box.left + box.width() * 0.5f, box.top);
			path->close();
			break;
		case ScrollBarButtonDirection::down:
			path->move_to(box.left, box.top);
			path->line_to(box.right, box.top);
			path->line_to(box.left + box.width() * 0.5f, box.bottom);
			path->close();
			break;
		}
		path->fill(canvas, Colorf(134, 137, 153));
	}

	void ScrollBarButtonBaseView::set_direction(ScrollBarButtonDirection new_dir)
	{
		if (new_dir != direction)
		{
			direction = new_dir;
			set_needs_render();
		}
	}

	void ScrollBarBaseViewImpl::update_thumb_state()
	{
		set_standard_state(thumb.get(), _state_disabled, _state_thumb_hot, _state_thumb_pressed);
	}
	void ScrollBarBaseViewImpl::update_track_state()
	{
		set_standard_state(track.get(), _state_disabled, _state_track_hot, _state_track_pressed);
	}
	void ScrollBarBaseViewImpl::update_decrement_state()
	{
		set_standard_state(button_decrement.get(), _state_disabled, _state_decrement_hot, _state_decrement_pressed);
	}
	void ScrollBarBaseViewImpl::update_increment_state()
	{
		set_standard_state(button_increment.get(), _state_disabled, _state_increment_hot, _state_increment_pressed);
	}

	void ScrollBarBaseViewImpl::set_standard_state(View *view, bool state_disabled, bool state_hot, bool state_pressed)
	{
		bool target_hot = false;
		bool target_disabled = false;
		bool target_pressed = false;

		if (state_disabled)
		{
			target_disabled = true;
		}
		else if (state_pressed)
		{
			target_pressed = true;
		}
		else if (state_hot)
		{
			target_hot = true;
		}

		view->set_state_cascade("hot", target_hot);
		view->set_state_cascade("pressed", target_pressed);
		view->set_state_cascade("disabled", target_disabled);
	}

	void ScrollBarBaseViewImpl::on_focus_gained(FocusChangeEvent &e)
	{
	}

	void ScrollBarBaseViewImpl::on_focus_lost(FocusChangeEvent &e)
	{
		mouse_down_mode = mouse_down_none;
		scroll_timer->stop();
	}

	void ScrollBarBaseViewImpl::on_activated(ActivationChangeEvent &e)
	{
	}

	void ScrollBarBaseViewImpl::on_deactivated(ActivationChangeEvent &e)
	{
		mouse_down_mode = mouse_down_none;
		scroll_timer->stop();
	}

	void ScrollBarBaseViewImpl::on_pointer_track_press(PointerEvent &e)
	{
		if (_state_disabled)
			return;
		float mouse_pos;
		Rectf thumb_geometry(thumb->geometry().content_box());
		float thumb_position;
		if (scrollbar->horizontal())
		{
			mouse_pos = e.pos(track.get()).x;
			thumb_position = thumb_geometry.left + thumb_geometry.width() * 0.5f;
			timer_target_position = min_pos + (mouse_pos + thumb_geometry.width() * 0.5f) * thumb_units_per_pixel();
		}
		else
		{
			mouse_pos = e.pos(track.get()).y;
			thumb_position = thumb_geometry.top + thumb_geometry.height() * 0.5f;
			timer_target_position = min_pos + (mouse_pos + thumb_geometry.height() * 0.5f) * thumb_units_per_pixel();
		}

		if (mouse_pos < thumb_position)
		{
			mouse_down_mode = mouse_down_track_decr;
			timer_step_size = -page_step;
		}
		else
		{
			mouse_down_mode = mouse_down_track_incr;
			timer_step_size = page_step;
		}

		scroll_timer_expired();

	}

	void ScrollBarBaseViewImpl::on_pointer_track_release(PointerEvent &e)
	{
		if (_state_disabled)
			return;
		mouse_down_mode = mouse_down_none;
		scroll_timer->stop();

	}

	void ScrollBarBaseViewImpl::on_pointer_thumb_press(PointerEvent &e)
	{
		if (_state_disabled)
			return;
		_state_thumb_pressed = true;
		update_thumb_state();

		mouse_down_mode = mouse_down_thumb_drag;
		thumb_move_start_position = pos;
		mouse_drag_start_pos = e.pos(track.get());

		e.stop_propagation(); // prevent track press reacting to this event
	}

	void ScrollBarBaseViewImpl::on_pointer_thumb_release(PointerEvent &e)
	{
		if (_state_disabled)
			return;
		_state_thumb_pressed = false;
		update_thumb_state();
		mouse_down_mode = mouse_down_none;

		e.stop_propagation(); // prevent track release reacting to this event
	}

	void ScrollBarBaseViewImpl::on_pointer_decrement_press(PointerEvent &e)
	{
		if (_state_disabled)
			return;
		_state_decrement_pressed = true;
		update_decrement_state();
		mouse_down_mode = mouse_down_button_decr;
		timer_step_size = -line_step;
		scroll_timer_expired();
	}

	void ScrollBarBaseViewImpl::on_pointer_decrement_release(PointerEvent &e)
	{
		if (_state_disabled)
			return;
		_state_decrement_pressed = false;
		update_decrement_state();
		mouse_down_mode = mouse_down_none;
		scroll_timer->stop();
	}

	void ScrollBarBaseViewImpl::on_pointer_increment_press(PointerEvent &e)
	{
		if (_state_disabled)
			return;
		_state_increment_pressed = true;
		update_increment_state();
		mouse_down_mode = mouse_down_button_incr;
		timer_step_size = line_step;
		scroll_timer_expired();
	}

	void ScrollBarBaseViewImpl::on_pointer_increment_release(PointerEvent &e)
	{
		if (_state_disabled)
			return;
		_state_increment_pressed = false;
		update_increment_state();
		mouse_down_mode = mouse_down_none;
		scroll_timer->stop();
	}

	void ScrollBarBaseViewImpl::on_pointer_move(PointerEvent &e)
	{
		if (_state_disabled)
			return;
		if (mouse_down_mode != mouse_down_thumb_drag)
			return;

		Pointf mouse_pos = e.pos(track.get());
		Rectf track_geometry = track->geometry().content_box();

		double last_position = pos;

		if (mouse_pos.x < -100 || mouse_pos.x > track_geometry.width() + 100 || mouse_pos.y < -100 || mouse_pos.y > track_geometry.height() + 100)
		{
			pos = thumb_move_start_position;
		}
		else
		{
			if (scrollbar->horizontal())
			{
				double delta = (mouse_pos.x - mouse_drag_start_pos.x);
				pos = thumb_move_start_position + delta * thumb_units_per_pixel();
			}
			else
			{
				double delta = (mouse_pos.y - mouse_drag_start_pos.y);
				pos = thumb_move_start_position + delta * thumb_units_per_pixel();
			}
		}

		pos = std::max(std::min(pos, max_pos), min_pos);

		if (last_position != pos)
		{
			scrollbar->set_needs_layout();
			sig_scroll();
		}
	}

	void ScrollBarBaseViewImpl::scroll_timer_expired()
	{
		if (_state_disabled)
			return;
		if ((mouse_down_mode == mouse_down_none) || (mouse_down_mode == mouse_down_thumb_drag))
			return;

		double last_position = pos;
		pos += timer_step_size;

		if (mouse_down_mode == mouse_down_track_decr)
		{
			if (pos < timer_target_position)
				pos = timer_target_position;
		}
		if (mouse_down_mode == mouse_down_track_incr)
		{
			if (pos > timer_target_position)
				pos = timer_target_position;
		}

		if (pos > max_pos)
			pos = max_pos;
		if (pos < min_pos)
			pos = min_pos;

		if (last_position != pos)
		{
			scrollbar->set_needs_layout();
			sig_scroll();
		}
		scroll_timer->start(300, false);
	}

	void ScrollBarBaseViewImpl::update_pos(ScrollBarBaseView *view, double new_pos, double new_min, double new_max)
	{
		bool changed = new_min != min_pos || new_max != max_pos || new_pos != pos;
		if (changed)
		{
			min_pos = new_min;
			max_pos = new_max;
			pos = new_pos;
			view->set_needs_layout();
		}
	}
}
