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
**    Mark Page
*/

#pragma once

#include "vec3.h"
#include "angle.h"

namespace uicore
{
	/// \brief Quaternion
	///
	/// These quaternion templates are defined for: float (Quaternionf), double (Quaterniond)
	template<typename Type>
	class Quaternionx
	{
	public:
		/// \brief The imaginary vector part
		Type x = Type(0);
		Type y = Type(0);
		Type z = Type(0);

		/// \brief The real scalar part
		Type w = Type(1);

		/// Constructs a quaternion
		Quaternionx() { }
		Quaternionx(const Quaternionx &other) : x(other.x), y(other.y), z(other.z), w(other.w) { }
		Quaternionx(Type x, Type y, Type z, Type w) : x(x), y(y), z(z), w(w) { }
		Quaternionx(const Vec4<Type> &quat) : x(quat.x), y(quat.y), z(quat.z), w(quat.w) { }
		explicit Quaternionx(const Mat4<Type> &rotation_matrix);

		static Quaternionx<Type> euler(Type x, Type y, Type z, EulerOrder order = EulerOrder::yxz);
		static Quaternionx<Type> euler(const Vec3<Type> &xyz, EulerOrder order = EulerOrder::yxz);
		static Quaternionx<Type> axis_angle(Type angle, const Vec3f &axis);

		static Quaternionx<Type> multiply(const Quaternionx<Type> &quaternion_1, const Quaternionx<Type> &quaternion_2);

		/// \brief Calculates the shortest arc quaternion between two vectors
		static Quaternionx<Type> rotation_between(Vec3<Type> v0, Vec3<Type> v1);

		/// \brief Calculates the shortest arc quaternion between two vectors
		static Quaternionx<Type> rotation_between(Vec4<Type> v0, Vec4<Type> v1);

		/// \brief Linear Quaternion Interpolation
		///
		/// \param quaternion_initial = Source quaternion
		/// \param quaternion_final = Destination quaternion
		/// \param lerp_time = Time in the range of 0.0 to 1.0
		static Quaternionx<Type> lerp(const Quaternionx<Type> &quaternion_initial, const Quaternionx<Type> &quaternion_final, Type lerp_time);

		/// \brief Spherical Quaternion Interpolation
		///
		/// \param quaternion_initial = Source quaternion
		/// \param quaternion_final = Destination quaternion
		/// \param slerp_time = Time in the range of 0.0 to 1.0
		static Quaternionx<Type> slerp(const Quaternionx<Type> &quaternion_initial, const Quaternionx<Type> &quaternion_final, Type slerp_time);

		/// \brief Convert the quaternion to a rotation matrix
		///
		/// This function assumes that the quarternion is normalized.
		Mat4<Type> to_matrix() const;

		/// Return quaternion as a vector
		Vec4<Type> to_vec4() const { return Vec4<Type>(x, y, z, w); }

		/// \brief Get the quaternion magnitude
		Type magnitude() const;

		Quaternionx<Type> &rotate(Type angle, const Vec3f &axis);

		Quaternionx<Type> &rotate(Type euler_x, Type euler_y, Type euler_z, EulerOrder order);

		/// \brief Normalizes this quaternion
		///
		/// \return reference to this object
		Quaternionx<Type> &normalize();

		/// \brief Inverse this quaternion
		///
		/// This is the same as the conjugate of a quaternion
		///
		/// \return reference to this object
		Quaternionx<Type> &inverse();

		/// \brief Rotates vector by this quaternion.
		///
		/// \param v = Vertex to rotate
		///
		/// \return rotated vector
		Vec3<Type> rotate_vector(const Vec3<Type> &v) const;

		Vec4<Type> rotate_vector(const Vec4<Type> &v) const;

		/// \brief Normalizes this quaternion
		///
		/// \return reference to this object
		static Quaternionx<Type> normalize(Quaternionx<Type> q) { return q.normalize(); }

		/// \brief Inverse this quaternion
		///
		/// This is the same as the conjugate of a quaternion
		///
		/// \return reference to this object
		static Quaternionx<Type> inverse(Quaternionx<Type> q) { return q.inverse(); }

		operator Vec4<Type>() const { return to_vec4(); }

		/// \brief Multiplication operator.
		Quaternionx<Type> operator *(const Quaternionx<Type> &mult) const { return Quaternionx<Type>::multiply(*this, mult); }

		Quaternionx<Type> operator *(const Mat4<Type> &matrix) const;

		Vec3<Type> operator *(const Vec3<Type> &v) const { return rotate_vector(v); }
		Vec4<Type> operator *(const Vec4<Type> &v) const { return rotate_vector(v); }

		/// \brief Less operator.
		bool operator<(const Quaternionx<Type> &other) const
		{
			if (x != other.x) return x < other.x;
			else if (y != other.y) return y < other.y;
			else if (z != other.z) return z < other.z;
			else return w < other.w;
		}

		/// \brief Greater operator.
		bool operator>(const Quaternionx<Type> &other) const
		{
			if (x != other.x) return x > other.x;
			else if (y != other.y) return y > other.y;
			else if (z != other.z) return z > other.z;
			else return w > other.w;
		}

		/// \brief Less equal operator.
		bool operator<=(const Quaternionx<Type> &other) const { return *this < other || *this == other; }

		/// \brief Greater equal operator.
		bool operator>=(const Quaternionx<Type> &other) const { return *this > other || *this == other; }

		/// \brief Equal operator.
		bool operator==(const Quaternionx<Type> &other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }

		/// \brief Not equal operator.
		bool operator!=(const Quaternionx<Type> &other) const { return x != other.x || y != other.y || z != other.z || w == other.w; }
	};

	typedef Quaternionx<float> Quaternionf;
	typedef Quaternionx<double> Quaterniond;
}
