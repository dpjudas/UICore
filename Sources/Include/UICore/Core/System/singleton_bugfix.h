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

#include <memory>
#include <functional>

namespace uicore
{
	/// \brief Workaround for broken destruction of static member variables in MSVC
	///
	/// The C++ runtime in Visual Studio 2013 and 2015 deinitializes the threading library prematurely.
	/// This causes std::thread and other standard library classes to not work in static member variables.
	///
	/// This SingletonBugfix and the Singleton template allows an application to deinitialize all
	/// static member variables wrapped by the Singleton template by calling SingletonBugfix::deinitialize()
	class SingletonBugfix
	{
	public:
		static void deinitialize();
		static void add_destructor(std::function<void()> destructor);
	};
	
	/// \brief Workaround for broken destruction of static member variables in MSVC
	template<typename T>
	class Singleton
	{
	public:
		Singleton() : value(new T)
		{
			SingletonBugfix::add_destructor([this]() { value.reset(); });
		}

		T *get() { return value.get(); }
		T *operator->() { return value.get(); }

	private:
		Singleton(const Singleton &) = delete;
		Singleton &operator=(const Singleton &) = delete;

		std::unique_ptr<T> value;
	};
}
