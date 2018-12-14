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
#include "UICore/UI/StandardViews/button_view.h"
#include "UICore/UI/StandardViews/image_view.h"
#include "button_view_impl.h"
#include "UICore/UI/Events/pointer_event.h"

namespace uicore
{
	void ButtonBaseViewImpl::update_state()
	{
		bool target_hot = false;
		bool target_disabled = false;
		bool target_pressed = false;

		if (_state_disabled)
		{
			target_disabled = true;
		}
		else if (_state_pressed)
		{
			target_pressed = true;
		}
		else if (_state_hot)
		{
			target_hot = true;
		}

		button->theme()->set_state("hot", target_hot);
		button->theme()->set_state("pressed", target_pressed);
		button->theme()->set_state("disabled", target_disabled);
	}

	void ButtonBaseViewImpl::on_pointer_press(PointerEvent &e)
	{
		if (_state_disabled || e.button() != PointerButton::left)
			return;
		_state_pressed = true;
		update_state();
	}

	void ButtonBaseViewImpl::on_pointer_release(PointerEvent &e)
	{
		_state_pressed = false;
		if (_state_disabled || e.button() != PointerButton::left)
			return;
		update_state();

		if (button->geometry().border_box().contains(e.pos(button) + button->geometry().content_box().position()))	// Only allow click when mouse released over component
			sig_clicked();
	}
}
