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

#include "view_event_handler.h"
#include <memory>

namespace uicore
{
	class EventUI;
	class PointerEvent;
	class ActivationChangeEvent;
	class FocusChangeEvent;
	class KeyEvent;
	class View;
	class ViewActionImpl;

	/// Recognizes actions in a view and captures input for the duration of the action
	class ViewAction : public std::enable_shared_from_this<ViewAction>, public ViewEventHandler
	{
	public:
		ViewAction();
		virtual ~ViewAction();

		/// Returns the view the action is attached to
		View *view() const;

		template<typename T>
		T *view() const { return dynamic_cast<T*>(view<View>()); }

		/// Removes recognizer from the view it is attached to
		void remove_from_view();

		/// Captures events until end is called
		void begin_action();

		/// Releases capture of events
		void end_action();

		/// Returns true if the action is capturing events
		bool action_active() const;

	private:
		ViewAction(const ViewAction &) = delete;
		ViewAction &operator=(const ViewAction &) = delete;

		std::unique_ptr<ViewActionImpl> impl;

		friend class ViewTree;
		friend class View;
		friend class ViewActionImpl;
	};
}
