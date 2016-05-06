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
#include "pi.h"

namespace uicore
{
	/// \brief Euler angle rotation order
	enum class EulerOrder
	{
		xyz,
		xzy,
		yzx,
		yxz,
		zxy,
		zyx
	};

	/// Convert a quantity in radians to degrees
	inline float degrees(float radians) { return radians * 180.0f * (float)(1.0 / PI); }
	inline double degrees(double radians) { return radians * 180.0 * (1.0 / PI_D); }

	/// Convert a quantity in degrees to radians
	inline float radians(float degrees) { return degrees * PI * (float)(1.0 / 180.0); }
	inline double radians(double degrees) { return degrees * PI_D * (1.0 / 180.0); }

	/// \brief Converts angle to range [0,360] degrees.
	float normalize_360(float degrees);
	double normalize_360(double degrees);

	/// \brief Converts angle to range [-180,180] degrees.
	float normalize_180(float degrees);
	double normalize_180(double degrees);

	/// \brief Converts angle to range [0,2*PI] degrees.
	float normalize_2pi(float radians);
	double normalize_2pi(double radians);

	/// \brief Converts angle to range [-PI,PI] degrees.
	float normalize_pi(float radians);
	double normalize_pi(double radians);
}
