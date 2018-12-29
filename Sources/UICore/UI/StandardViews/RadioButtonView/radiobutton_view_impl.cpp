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
**    Mark Page
*/

#include "UICore/precomp.h"
#include "UICore/UI/StandardViews/radiobutton_view.h"
#include "UICore/Display/2D/path.h"
#include "UICore/Display/System/timer.h"
#include "UICore/Display/2D/brush.h"
#include "UICore/UI/Events/pointer_event.h"
#include <algorithm>
#include "radiobutton_view_impl.h"

namespace uicore
{
	void RadioButtonBaseView_Impl::update_state()
	{
		bool target_checked = false;
		bool target_checked_hot = false;
		bool target_checked_disabled = false;
		bool target_checked_pressed = false;
		bool target_unchecked = false;
		bool target_unchecked_hot = false;
		bool target_unchecked_disabled = false;
		bool target_unchecked_pressed = false;

		if (_state_selected)
		{
			if (_state_disabled)
			{
				target_checked_disabled = true;
			}
			else if (_state_pressed)
			{
				target_checked_pressed = true;
			}
			else if (_state_hot)
			{
				target_checked_hot = true;
			}
			else
			{
				target_checked = true;
			}
		}
		else
		{
			if (_state_disabled)
			{
				target_unchecked_disabled = true;
			}
			else if (_state_pressed)
			{
				target_unchecked_pressed = true;
			}
			else if (_state_hot)
			{
				target_unchecked_hot = true;
			}
			else
			{
				target_unchecked = true;
			}

		}

		radio->theme()->set_state("checked", target_checked);
		radio->theme()->set_state("checked_hot", target_checked_hot);
		radio->theme()->set_state("checked_pressed", target_checked_pressed);
		radio->theme()->set_state("checked_disabled", target_checked_disabled);
		//radio->theme()->set_state("unchecked", target_unchecked);	(The default state)
		radio->theme()->set_state("unchecked_hot", target_unchecked_hot);
		radio->theme()->set_state("unchecked_pressed", target_unchecked_pressed);
		radio->theme()->set_state("unchecked_disabled", target_unchecked_disabled);
	}

	void RadioButtonBaseView_Impl::on_pointer_press(PointerEvent &e)
	{
		if (_state_disabled || e.button() != PointerButton::left)
			return;
		_state_pressed = true;
		update_state();
	}

	void RadioButtonBaseView_Impl::on_pointer_release(PointerEvent &e)
	{
		_state_pressed = false;
		if (_state_disabled || e.button() != PointerButton::left)
			return;

		if (!_state_selected)
		{
			if (radio->geometry().border_box().contains(e.pos(radio) + radio->geometry().content_box().position()))	// Only allow click when mouse released over component
			{
				_state_selected = true;
				release_group_checked();
				sig_selected();
			}
		}

		update_state();
	}

	void RadioButtonBaseView_Impl::release_group_checked()
	{
#if 0
		auto parent = radio->parent();
		if (parent)
		{
			for (auto &view : parent->children())
			{
				if (view.get() != radio)
				{
					RadioButtonView *alt_radio = dynamic_cast<RadioButtonView*>(view.get());
					if (alt_radio)
					{
						if (alt_radio->impl->_group == _group)
						{
							alt_radio->set_selected(false);
						}
					}
				}
			}
		}
#endif
	}
}
