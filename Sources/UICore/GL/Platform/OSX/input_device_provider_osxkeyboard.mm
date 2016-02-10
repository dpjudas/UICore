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
**    James Lammlein
**    Magnus Norddahl
**    Harry Storbacka
*/

#include "UICore/precomp.h"
#include "input_device_provider_osxkeyboard.h"

#include "UICore/Core/Text/text.h"
#include "UICore/Display/Window/display_window.h"
#include "UICore/Display/Window/input_event.h"
#include "UICore/Display/Window/keys.h"

namespace uicore
{

/////////////////////////////////////////////////////////////////////////////
// InputDeviceProvider_OSXKeyboard construction:

InputDeviceProvider_OSXKeyboard::InputDeviceProvider_OSXKeyboard(OpenGLWindowProvider *window)
    : sig_provider_event(nullptr), window(window)
{
    for (int i = 0; i < uicore::keycode_count; ++i)
    {
        key_down_map[i] = false;
    }
}

InputDeviceProvider_OSXKeyboard::~InputDeviceProvider_OSXKeyboard()
{
	dispose();
}

/////////////////////////////////////////////////////////////////////////////
// InputDeviceProvider_OSXKeyboard attributes:

bool InputDeviceProvider_OSXKeyboard::keycode(int keycode) const
{
    throw_if_disposed();

    bool result = false;

    if (keycode < uicore::keycode_count)
    {
        result = key_down_map[keycode];
    }

    return result;
}

std::string InputDeviceProvider_OSXKeyboard::key_name(int virtual_key) const
{
    //
    // TODO: Reimplement.
    //

    throw_if_disposed();
    return std::string();
}

float InputDeviceProvider_OSXKeyboard::axis(int index) const
{
    throw_if_disposed();
    return 0.0f;
}

std::string InputDeviceProvider_OSXKeyboard::name() const
{
    throw_if_disposed();
    return "System Keyboard";
}

std::string InputDeviceProvider_OSXKeyboard::device_name() const
{
    throw_if_disposed();
    return "System Keyboard";
}

std::vector<int> InputDeviceProvider_OSXKeyboard::axis_ids() const
{
    throw_if_disposed();
    return std::vector<int>();
}

int InputDeviceProvider_OSXKeyboard::button_count() const
{
    throw_if_disposed();
    return -1;
}

/////////////////////////////////////////////////////////////////////////////
// InputDeviceProvider_OSXKeyboard operations:

/////////////////////////////////////////////////////////////////////////////
// InputDeviceProvider_OSXKeyboard implementation:

void InputDeviceProvider_OSXKeyboard::on_dispose()
{
}

void InputDeviceProvider_OSXKeyboard::on_key_event(const uicore::InputCode& keycode, const uicore::InputEvent::Type& type)
{
    if (keycode < uicore::keycode_count)
    {
        if (type == uicore::InputEvent::pressed)
        {
            key_down_map[keycode] = true;
        }
        else if (type == uicore::InputEvent::released)
        {
            key_down_map[keycode] = false;
        }
    }
}

}