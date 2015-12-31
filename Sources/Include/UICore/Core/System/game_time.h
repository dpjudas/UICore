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

#include <cstdint>
#include <memory>

namespace uicore
{
	class GameTime_Impl;

	/// \brief Tracks time elapsed in various forms useful for games
	class GameTime
	{
	public:
		/// \brief GameTime constructor
		///
		/// \param ticks_per_second = Number of ticks per second
		/// \param max_updates_per_second = Maximum number of updates per second (aka FPS, frames per second). 0 = Unlimited
		GameTime(int ticks_per_second = 20, int max_updates_per_second = 0);

		/// \brief Returns the time elapsed in seconds since last update.
		float time_elapsed() const;

		/// \brief Returns the time elapsed in seconds since last update in milliseconds
		int time_elapsed_ms() const;

		/// \brief Returns the number of ticks that elapsed since last update.
		int ticks_elapsed() const;

		/// \brief Returns the time elapsed per tick.
		float tick_time_elapsed() const;

		/// \brief Returns the time elapsed per tick in milliseconds
		int tick_time_elapsed_ms() const;

		/// \brief Returns the current time between ticks as a normalized number.
		///
		/// If the current time is half-way between two tick updates this function returns 0.5,
		/// at the start of a tick its 0.0, and at the end its 1.0.
		float tick_interpolation_time() const;

		/// \brief Returns the number of updates that occurred every second (aka fps, frames per second)
		float updates_per_second() const;

		/// \brief Returns the number of seconds since this class was reset()
		float current_time() const;

		/// \brief Returns the number of microseconds since this class was reset()
		uint64_t current_time_microseconds() const;

		/// \brief Returns the number of milliseconds since this class was reset()
		uint64_t current_time_ms() const;

		/// \brief Increases or decreases tick speed by the specified amount of microseconds per second
		void set_tick_time_adjustment(int64_t microseconds);

		/// \brief Updates time data for the frame to be rendered
		///
		/// Call this function once at the start of a frame.
		void update();

		/// \brief Resets the timer.
		void reset();

	private:
		std::shared_ptr<GameTime_Impl> impl;
	};
}
