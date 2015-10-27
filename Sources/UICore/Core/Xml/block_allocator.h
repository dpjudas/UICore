/*
**  UICore SDK
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

namespace uicore
{
	class BlockAllocator_Impl;

	class BlockAllocator
	{
	public:
		BlockAllocator();

		void *allocate(int size);
		void free();

	private:
		std::shared_ptr<BlockAllocator_Impl> impl;
	};

	class BlockAllocated
	{
	public:
		void *operator new(size_t size, BlockAllocator *allocator);
		void operator delete(void *data, size_t size);
		void operator delete(void *data, BlockAllocator *allocator);
	};
}
