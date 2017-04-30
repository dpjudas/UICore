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
#include "vertex_array_buffer.h"

namespace uicore
{
	class GraphicContext;
	class StagingBuffer;

	/// \brief Element Array Buffer
	class ElementArrayBuffer
	{
	public:
		/// \brief Constructs a buffer holding vertex indices
		static std::shared_ptr<ElementArrayBuffer> create(const std::shared_ptr<GraphicContext> &gc, int size, BufferUsage usage = usage_static_draw);
		static std::shared_ptr<ElementArrayBuffer> create(const std::shared_ptr<GraphicContext> &gc, const void *data, int size, BufferUsage usage = usage_static_draw);

		/// \brief Uploads data to element array buffer.
		///
		/// The size specified must match the size of the buffer and is only included to help guard against buffer overruns.
		virtual void upload_data(const std::shared_ptr<GraphicContext> &gc, const void *data, int size) = 0;

		/// \brief Copies data from transfer buffer
		virtual void copy_from(const std::shared_ptr<GraphicContext> &gc, const std::shared_ptr<StagingBuffer> &buffer, int dest_pos = 0, int src_pos = 0, int size = -1) = 0;

		/// \brief Copies data to transfer buffer
		virtual void copy_to(const std::shared_ptr<GraphicContext> &gc, const std::shared_ptr<StagingBuffer> &buffer, int dest_pos = 0, int src_pos = 0, int size = -1) = 0;
	};
}
