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
**    Harry Storbacka
**    Magnus Norddahl
**    Kenneth Gangstoe
**    Mark Page
*/

#include "UICore/precomp.h"
#include "UICore/UI/StandardViews/spin_view.h"
#include "UICore/UI/Style/style_property_parser.h"
#include "UICore/Display/2D/path.h"
#include "UICore/Display/System/timer.h"
#include "UICore/Display/2D/brush.h"
#include "UICore/UI/Events/pointer_event.h"
#include "UICore/UI/Events/focus_change_event.h"
#include "UICore/UI/Events/activation_change_event.h"
#include <algorithm>
#include "spin_view_impl.h"

namespace uicore
{
	SpinBaseView::SpinBaseView() : impl(std::make_shared<SpinBaseView_Impl>())
	{
		impl->spin = this;

		impl->button_decrement = std::make_shared<View>();
		impl->button_increment = std::make_shared<View>();

		add_child(impl->button_decrement);
		add_child(impl->button_increment);

		slots.connect(impl->button_decrement->sig_pointer_press(), [this](PointerEvent *e) { impl->on_pointer_decrement_press(*e); });
		slots.connect(impl->button_decrement->sig_pointer_release(), [this](PointerEvent *e) { impl->on_pointer_decrement_release(*e); });
		slots.connect(impl->button_increment->sig_pointer_press(), [this](PointerEvent *e) { impl->on_pointer_increment_press(*e); });
		slots.connect(impl->button_increment->sig_pointer_release(), [this](PointerEvent *e) { impl->on_pointer_increment_release(*e); });

		slots.connect(sig_focus_gained(), [this](FocusChangeEvent *e) { impl->on_focus_gained(*e); });
		slots.connect(sig_focus_lost(), [this](FocusChangeEvent *e) { impl->on_focus_lost(*e); });
		slots.connect(sig_activated(), [this](ActivationChangeEvent *e) { impl->on_activated(*e); });
		slots.connect(sig_deactivated(), [this](ActivationChangeEvent *e) { impl->on_deactivated(*e); });

		slots.connect(impl->button_decrement->sig_pointer_enter(), [&](PointerEvent *e) {impl->_state_decrement_hot = true; impl->update_decrement_state(); });
		slots.connect(impl->button_decrement->sig_pointer_leave(), [&](PointerEvent *e) {impl->_state_decrement_hot = false; impl->update_decrement_state(); });
		slots.connect(impl->button_increment->sig_pointer_enter(), [&](PointerEvent *e) {impl->_state_increment_hot = true; impl->update_increment_state(); });
		slots.connect(impl->button_increment->sig_pointer_leave(), [&](PointerEvent *e) {impl->_state_increment_hot = false; impl->update_increment_state(); });

		impl->timer->func_expired() = uicore::bind_member(impl.get(), &SpinBaseView_Impl::timer_expired);
	}

	std::shared_ptr<View> SpinBaseView::button_decrement() const
	{
		return impl->button_decrement;
	}

	std::shared_ptr<View> SpinBaseView::button_increment() const
	{
		return impl->button_increment;
	}

	bool SpinBaseView::disabled() const
	{
		return impl->_state_disabled;
	}

	void SpinBaseView::set_disabled()
	{
		if (!impl->_state_disabled)
		{
			impl->_state_disabled = true;
			impl->update_increment_state();
			impl->update_decrement_state();

			impl->mouse_down_mode = SpinBaseView_Impl::mouse_down_none;
			impl->timer->stop();
		}
	}

	void SpinBaseView::set_enabled()
	{
		if (impl->_state_disabled)
		{
			impl->_state_disabled = false;
			impl->update_increment_state();
			impl->update_decrement_state();
		}
	}

	double SpinBaseView::value() const
	{
		return impl->_value;
	}

	double SpinBaseView::minimum() const
	{
		return impl->_minimum;
	}

	double SpinBaseView::maximum() const
	{
		return impl->_maximum;
	}

	void SpinBaseView::set_value(double value)
	{
		if (impl->_value != value)
		{
			impl->_value = value;
		}
	}

	void SpinBaseView::set_ranges(double min, double max)
	{
		if ((impl->_minimum != min) || (impl->_maximum != max))
		{
			impl->_minimum = min;
			impl->_maximum = max;
		}
	}

	void SpinBaseView::set_step_size(double step_size)
	{
		if (impl->_step_size != step_size)
		{
			impl->_step_size = step_size;
		}
	}

	void SpinBaseView::set_number_of_decimal_places(int decimal_places)
	{
		if (impl->_decimal_places != decimal_places)
		{
			impl->_decimal_places = decimal_places;
		}
	}
}
