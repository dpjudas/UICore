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
**  Note: Some of the libraries ClanLib may link to may have additional
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
	/// \brief General purpose data buffer.
	class DataBuffer
	{
	public:
		/// \brief Constructs a data buffer
		static std::shared_ptr<DataBuffer> create(size_t size);
		static std::shared_ptr<DataBuffer> create(const void *data, size_t size);

		/// \brief Returns a pointer to the data.
		virtual char *data() = 0;
		virtual const char *data() const = 0;

		template<typename Type> Type *data() { return reinterpret_cast<Type*>(data()); }
		template<typename Type> const Type *data() const { return reinterpret_cast<const Type*>(data()); }

		/// \brief Returns the size of the data.
		virtual size_t size() const = 0;

		/// \brief Returns the capacity of the data buffer object.
		virtual size_t capacity() const = 0;

		/// \brief Resize the buffer.
		virtual void set_size(size_t size) = 0;

		/// \brief Preallocate enough memory.
		virtual void set_capacity(size_t capacity) = 0;

		/// \brief Copy buffer to a new buffer
		virtual std::shared_ptr<DataBuffer> copy(size_t pos, size_t size) = 0;
	};

	typedef std::shared_ptr<DataBuffer> DataBufferPtr;
}
