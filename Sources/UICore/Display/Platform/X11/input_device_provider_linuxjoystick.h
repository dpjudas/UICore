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

#pragma once

#include "UICore/Display/Window/input_device_provider.h"

struct js_event;

namespace uicore
{
	class X11Window;

	class InputDeviceProvider_LinuxJoystick : public InputDeviceProvider
	{
	public:
		InputDeviceProvider_LinuxJoystick(X11Window *window, const std::string &device);
		~InputDeviceProvider_LinuxJoystick();

		/// \brief Get the file descriptor of the joystick
		int get_fd() const;

		InputDevice::Type type() const override { return InputDevice::joystick; }
		bool keycode(int keycode) const override;
		std::string key_name(int id) const override;
		float axis(int index) const override;
		std::string name() const override;
		std::string device_name() const override;
		std::vector<int> axis_ids() const override;
		int button_count() const override;

		/// \brief Update device
		/// \param peek_only Treat as a request to see if an event would occur
		/// \return true when the device event has occurred
		bool poll(bool peek_only);

	private:
		void on_dispose() override;
		void process_event(js_event event);

		X11Window *window;
		std::string device;
		int fd;

		mutable std::vector<float> axis_states;
		mutable std::vector<bool>  button_states;

		std::string _name;

		mutable bool new_event;
	};
}
