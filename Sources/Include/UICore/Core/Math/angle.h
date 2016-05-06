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
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

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
	template<typename T> Vec2<T> degrees(const Vec2<T> &radians) { return Vec2<T>(degrees(radians.x), degrees(radians.y)); }
	template<typename T> Vec3<T> degrees(const Vec3<T> &radians) { return Vec3<T>(degrees(radians.x), degrees(radians.y), degrees(radians.z)); }
	template<typename T> Vec4<T> degrees(const Vec4<T> &radians) { return Vec4<T>(degrees(radians.x), degrees(radians.y), degrees(radians.z), degrees(radians.w)); }

	/// Convert a quantity in degrees to radians
	inline float radians(float degrees) { return degrees * PI * (float)(1.0 / 180.0); }
	inline double radians(double degrees) { return degrees * PI_D * (1.0 / 180.0); }
	template<typename T> Vec2<T> radians(const Vec2<T> &degrees) { return Vec2<T>(radians(degrees.x), radians(degrees.y)); }
	template<typename T> Vec3<T> radians(const Vec3<T> &degrees) { return Vec3<T>(radians(degrees.x), radians(degrees.y), radians(degrees.z)); }
	template<typename T> Vec4<T> radians(const Vec4<T> &degrees) { return Vec4<T>(radians(degrees.x), radians(degrees.y), radians(degrees.z), radians(degrees.w)); }

	/// \brief Converts angle to range [0,360] degrees.
	float normalize_360(float degrees);
	double normalize_360(double degrees);
	template<typename T> Vec2<T> normalize_360(const Vec2<T> &degrees) { return Vec2<T>(normalize_360(degrees.x), normalize_360(degrees.y)); }
	template<typename T> Vec3<T> normalize_360(const Vec3<T> &degrees) { return Vec3<T>(normalize_360(degrees.x), normalize_360(degrees.y), normalize_360(degrees.z)); }
	template<typename T> Vec4<T> normalize_360(const Vec4<T> &degrees) { return Vec4<T>(normalize_360(degrees.x), normalize_360(degrees.y), normalize_360(degrees.z), normalize_360(degrees.w)); }

	/// \brief Converts angle to range [-180,180] degrees.
	float normalize_180(float degrees);
	double normalize_180(double degrees);
	template<typename T> Vec2<T> normalize_180(const Vec2<T> &degrees) { return Vec2<T>(normalize_180(degrees.x), normalize_180(degrees.y)); }
	template<typename T> Vec3<T> normalize_180(const Vec3<T> &degrees) { return Vec3<T>(normalize_180(degrees.x), normalize_180(degrees.y), normalize_180(degrees.z)); }
	template<typename T> Vec4<T> normalize_180(const Vec4<T> &degrees) { return Vec4<T>(normalize_180(degrees.x), normalize_180(degrees.y), normalize_180(degrees.z), normalize_180(degrees.w)); }

	/// \brief Converts angle to range [0,2*PI] radians.
	float normalize_2pi(float radians);
	double normalize_2pi(double radians);
	template<typename T> Vec2<T> normalize_2pi(const Vec2<T> &radians) { return Vec2<T>(normalize_2pi(radians.x), normalize_2pi(radians.y)); }
	template<typename T> Vec3<T> normalize_2pi(const Vec3<T> &radians) { return Vec3<T>(normalize_2pi(radians.x), normalize_2pi(radians.y), normalize_2pi(radians.z)); }
	template<typename T> Vec4<T> normalize_2pi(const Vec4<T> &radians) { return Vec4<T>(normalize_2pi(radians.x), normalize_2pi(radians.y), normalize_2pi(radians.z), normalize_2pi(radians.w)); }

	/// \brief Converts angle to range [-PI,PI] radians.
	float normalize_pi(float radians);
	double normalize_pi(double radians);
	template<typename T> Vec2<T> normalize_pi(const Vec2<T> &radians) { return Vec2<T>(normalize_pi(radians.x), normalize_pi(radians.y)); }
	template<typename T> Vec3<T> normalize_pi(const Vec3<T> &radians) { return Vec3<T>(normalize_pi(radians.x), normalize_pi(radians.y), normalize_pi(radians.z)); }
	template<typename T> Vec4<T> normalize_pi(const Vec4<T> &radians) { return Vec4<T>(normalize_pi(radians.x), normalize_pi(radians.y), normalize_pi(radians.z), normalize_pi(radians.w)); }
}
