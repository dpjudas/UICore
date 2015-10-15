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
**    Harry Storbacka
**    Mark Page
*/

#include "UICore/precomp.h"
#include "UICore/Display/Window/display_window.h"
#include "UICore/Display/Window/display_window_description.h"
#include "UICore/Display/Window/input_device.h"
#include "UICore/Display/display_target.h"
#include "UICore/Display/display_target_provider.h"

namespace uicore
{
	std::shared_ptr<DisplayWindow> DisplayWindow::create(const std::string &title, float width, float height, bool start_fullscreen, bool allow_resize, int flipping_buffers)
	{
		DisplayWindowDescription description;
		description.set_title(title);
		description.set_size(Size(width, height), false);
		description.set_fullscreen(start_fullscreen);

		if (start_fullscreen)
		{
			description.show_caption(false);
		}

		description.set_allow_resize(allow_resize);
		description.set_flipping_buffers(flipping_buffers);

		return create(description);
	}

	std::shared_ptr<DisplayWindow> DisplayWindow::create(const DisplayWindowDescription &description)
	{
		auto target = DisplayTarget::get_current_target();
		if (!target)
			throw Exception("No display target set");

		return target->create_display_window(description);
	}

	const InputDevicePtr &DisplayWindow::input_device(const std::string &device_name) const
	{
		if (device_name == keyboard()->device_name())
			return keyboard();
		else if (device_name == mouse()->device_name())
			return mouse();

		const auto &game = game_controllers();
		for (auto &elem : game)
		{
			if (elem->device_name() == device_name)
				return elem;
		}

		static InputDevicePtr null_device;
		return null_device;
	}

	void DisplayWindow::set_visible(bool visible, bool activate)
	{
		if (visible)
			show(activate);
		else
			hide();
	}
}
