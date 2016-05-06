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

#include "UICore/precomp.h"
#include "UICore/Core/Math/angle.h"
#include "UICore/Core/Math/cl_math.h"

namespace uicore
{
	float normalize_360(float degrees)
	{
		degrees = std::fmod(degrees, 360.0f);
		if (degrees < 0.0f)
			degrees += 360.0f;
		return degrees;
	}

	double normalize_360(double degrees)
	{
		degrees = std::fmod(degrees, 360.0);
		if (degrees < 0.0)
			degrees += 360.0;
		return degrees;
	}

	/// \brief Converts angle to range [-180,180] degrees.
	float normalize_180(float degrees)
	{
		degrees = normalize_360(degrees);
		if (degrees > 180.0f)
			degrees -= 360.0f;
		return degrees;
	}

	double normalize_180(double degrees)
	{
		degrees = normalize_360(degrees);
		if (degrees > 180.0)
			degrees -= 360.0;
		return degrees;
	}

	/// \brief Converts angle to range [0,2*PI] degrees.
	float normalize_2pi(float radians)
	{
		radians = std::fmod(radians, PI * 2.0f);
		if (radians < 0.0f)
			radians += PI * 2.0f;
		return radians;
	}

	double normalize_2pi(double radians)
	{
		radians = std::fmod(radians, PI_D * 2.0);
		if (radians < 0.0)
			radians += PI * 2.0;
		return radians;
	}

	/// \brief Converts angle to range [-PI,PI] degrees.
	float normalize_pi(float radians)
	{
		radians = normalize_2pi(radians);
		if (radians > PI)
			radians -= PI * 2.0f;
		return radians;
	}

	double normalize_pi(double radians)
	{
		radians = normalize_2pi(radians);
		if (radians > PI)
			radians -= PI * 2.0;
		return radians;
	}
}
