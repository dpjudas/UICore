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

#include "staging_buffer.h"

namespace uicore
{
	/// \brief Typed access to a transfer buffer
	template<typename Type>
	class StagingVector
	{
	public:
		/// \brief Constructs a transfer vector
		StagingVector()
		{
		}

		StagingVector(const std::shared_ptr<StagingBuffer> &staging_buffer)
			: _buffer(staging_buffer)
		{
		}

		StagingVector(const std::shared_ptr<GraphicContext> &gc, int size, BufferUsage usage = usage_dynamic_copy)
			: _buffer(StagingBuffer::create(gc, size * sizeof(Type), usage))
		{
		}

		StagingVector(const std::shared_ptr<GraphicContext> &gc, Type *data, int size, BufferUsage usage = usage_dynamic_copy)
			: _buffer(StagingBuffer::create(gc, data, size * sizeof(Type), usage))
		{
		}

		StagingVector(const std::shared_ptr<GraphicContext> &gc, const std::vector<Type> &data, BufferUsage usage = usage_dynamic_copy)
			: _buffer(StagingBuffer::create(gc, data.empty() ? (Type*)0 : &data[0], data.size() * sizeof(Type), usage))
		{
		}

		/// Returns the buffer used by the vector
		const std::shared_ptr<StagingBuffer> &buffer() const { return _buffer; }

		operator const std::shared_ptr<StagingBuffer> &() const { return buffer(); }

		/// \brief Retrieves a pointer to the mapped buffer.
		Type *data() { return reinterpret_cast<Type*>(_buffer->data()); }

		/// \brief Maps buffer into system memory.
		void lock(const std::shared_ptr<GraphicContext> &gc, BufferAccess access) { _buffer->lock(gc, access); }

		/// \brief Unmaps buffer.
		void unlock() { _buffer->unlock(); }

		/// \brief Uploads data to transfer buffer.
		void upload_data(const std::shared_ptr<GraphicContext> &gc, int offset, const Type *data, int size)
		{
			_buffer->upload_data(gc, offset * sizeof(Type), data, size * sizeof(Type));
		}

		/// \brief Uploads data to transfer buffer.
		void upload_data(const std::shared_ptr<GraphicContext> &gc, int offset, const std::vector<Type> &data)
		{
			_buffer->upload_data(gc, offset * sizeof(Type), data.data(), data.size() * sizeof(Type));
		}

	private:
		std::shared_ptr<StagingBuffer> _buffer;
	};
}
