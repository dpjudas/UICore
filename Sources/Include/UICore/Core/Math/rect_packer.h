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
**    Kenneth Gangstoe
*/

#pragma once

#include <memory>
#include "rect.h"

namespace uicore
{
	class Size;

	/// \brief Generic rect packer class. Implements an algorithm to pack rects into groups efficiently.
	class RectPacker
	{
	public:
		/// \brief Allocation policy.
		enum AllocationPolicy
		{
			create_new_group,
			search_previous_groups,
			fail_if_full
		};

		struct AllocatedRect
		{
		public:
			AllocatedRect(int group_index, Rect rect) : group_index(group_index), rect(rect) {}
			int group_index;
			Rect rect;
		};

		/// \brief Constructs a rect group.
		static std::shared_ptr<RectPacker> create(const Size &max_group_size, AllocationPolicy policy = create_new_group);

		/// \brief Returns the allocation policy.
		virtual AllocationPolicy allocation_policy() const = 0;

		/// \brief Returns the max group size.
		virtual Size max_group_size() const = 0;

		/// \brief Returns the total amount of rects.
		virtual int total_rect_count() const = 0;

		/// \brief Returns the amount of rects in a group.
		virtual int rect_count(unsigned int group_index = 0) const = 0;

		/// \brief Returns the amount of rects used by group.
		virtual int group_count() const = 0;

		/// \brief Allocate space for another rect.
		virtual AllocatedRect add(const Size &size) = 0;
	};
}
