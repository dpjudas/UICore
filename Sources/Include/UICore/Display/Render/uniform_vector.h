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
#include "uniform_buffer.h"

namespace uicore
{
	/// \brief Typed access to an uniform buffer
	template<typename Type>
	class UniformVector
	{
	public:
		/// \brief Constructs a uniform vector
		UniformVector()
		{
		}

		UniformVector(const GraphicContextPtr &gc, int size, BufferUsage usage = usage_static_draw)
			: _buffer(UniformBuffer::create(gc, size * sizeof(Type), usage))
		{
		}

		UniformVector(const GraphicContextPtr &gc, Type *data, int size, BufferUsage usage = usage_static_draw)
			: _buffer(UniformBuffer::create(gc, data, size * sizeof(Type), usage))
		{
		}

		UniformVector(const GraphicContextPtr &gc, const std::vector<Type> &data, BufferUsage usage = usage_static_draw)
			: _buffer(UniformBuffer::create(gc, data.empty() ? (Type*)0 : &data[0], data.size() * sizeof(Type), usage))
		{
		}

		/// Returns the buffer used by the vector
		const UniformBufferPtr &buffer() const { return _buffer; }

		operator const UniformBufferPtr &() const { return buffer(); }

		/// \brief Uploads data to uniforms buffer.
		///
		/// The size specified must match the size of the buffer and is only included to help guard against buffer overruns.
		void upload_data(const GraphicContextPtr &gc, const Type *data, int size)
		{
			_buffer->upload_data(gc, data, size * sizeof(Type));
		}

		/// \brief Uploads data to uniforms buffer.
		void upload_data(const GraphicContextPtr &gc, const std::vector<Type> &data)
		{
			_buffer->upload_data(gc, data.data(), data.size() * sizeof(Type));
		}

		/// \brief Copies data from transfer buffer
		void copy_from(const GraphicContextPtr &gc, TransferVector<Type> &buffer, int dest_pos = 0, int src_pos = 0, int size = -1)
		{
			if (size != -1)
				size = size * sizeof(Type);
			_buffer->copy_from(gc, buffer, dest_pos * sizeof(Type), src_pos * sizeof(Type), size);
		}

		/// \brief Copies data to transfer buffer
		void copy_to(const GraphicContextPtr &gc, TransferVector<Type> &buffer, int dest_pos = 0, int src_pos = 0, int size = -1)
		{
			if (size != -1)
				size = size * sizeof(Type);
			_buffer->copy_to(gc, buffer, dest_pos * sizeof(Type), src_pos * sizeof(Type), size);
		}

	private:
		UniformBufferPtr _buffer;
	};
}
