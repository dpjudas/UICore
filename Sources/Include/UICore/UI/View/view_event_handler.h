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
*/

#pragma once

namespace uicore
{
	class EventUI;
	class PointerEvent;
	class ActivationChangeEvent;
	class FocusChangeEvent;
	class KeyEvent;
	class CloseEvent;

	/// Event sink interface for view events
	class ViewEventHandler
	{
	public:
		/// Handler for pointer press events
		virtual void pointer_press(PointerEvent *e) { }

		/// Handler for pointer double click events
		virtual void pointer_double_click(PointerEvent *e) { }

		/// Handler for pointer release events
		virtual void pointer_release(PointerEvent *e) { }

		/// Handler for pointer movement events
		virtual void pointer_move(PointerEvent *e) { }

		/// Handler for pointer enter events
		virtual void pointer_enter(PointerEvent *e) { }

		/// Handler for pointer leave events
		virtual void pointer_leave(PointerEvent *e) { }

		/// Handler for pointer proximity change events
		virtual void pointer_proximity_change(PointerEvent *e) { }

		/// Handler for when the application is activated
		virtual void activated(ActivationChangeEvent *e) { }

		/// Handler for when the application is deactivated
		virtual void deactivated(ActivationChangeEvent *e) { }

		/// Handler for focus gained events
		virtual void focus_gained(FocusChangeEvent *e) { }

		/// Handler for focus lost events
		virtual void focus_lost(FocusChangeEvent *e) { }

		/// Handler for key press events
		virtual void key_press(KeyEvent *e) { }

		/// Handler for key release events
		virtual void key_release(KeyEvent *e) { }

		/// Handler for close events
		virtual void close(CloseEvent *e) { }
	};
}
