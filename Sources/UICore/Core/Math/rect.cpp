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
**    Kenneth Gangstoe
**    Harry Storbacka
**    Mark Page
*/

#include "UICore/precomp.h"
#include "UICore/Core/Math/rect.h"

namespace uicore
{
	template<typename Type>
	Rectx<Type> Rectx<Type>::rot_bounds(const Vec2<Type> &hotspot, float angle) const
	{
		//Find the rotated positions of each corner
		Rectx<Type> retVal(*this);
		Vec2<Type> ul(retVal.left, retVal.top);
		ul.rotate(hotspot, angle);
		Vec2<Type> ur(retVal.right, retVal.top);
		ur.rotate(hotspot, angle);
		Vec2<Type> ll(retVal.left, retVal.bottom);
		ll.rotate(hotspot, angle);
		Vec2<Type> lr(retVal.right, retVal.bottom);
		lr.rotate(hotspot, angle);

		//Use the sidemost corners as the bounds of the new rectangle
		retVal.left = min(min(ul.x, ur.x), min(ll.x, lr.x));
		retVal.right = max(max(ul.x, ur.x), max(ll.x, lr.x));
		retVal.top = min(min(ul.y, ur.y), min(ll.y, lr.y));
		retVal.bottom = max(max(ul.y, ur.y), max(ll.y, lr.y));

		return retVal;
	}

	template<typename Type>
	Rectx<Type> Rectx<Type>::rot_bounds(Origin origin, Type x, Type y, float angle) const
	{
		return rot_bounds(
			Vec2<Type>(left, top) + Vec2<Type>::calc_origin(origin, size()) + Vec2<Type>(x, y),
			angle);
	}

	// Explicit instantiate the versions we use:

	template class Rectx<int>;
	template class Rectx<float>;
	template class Rectx<double>;
}
