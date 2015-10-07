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
#include "../Image/buffer_usage.h"

namespace uicore
{
	class GraphicContext;
	class TransferBuffer;
	typedef std::shared_ptr<GraphicContext> GraphicContextPtr;
	typedef std::shared_ptr<TransferBuffer> TransferBufferPtr;

	/// \brief Vertex Array Buffer
	class VertexArrayBuffer
	{
	public:
		/// \brief Constructs a vertex array buffer
		static std::shared_ptr<VertexArrayBuffer> create(const GraphicContextPtr &gc, int size, BufferUsage usage = usage_static_draw);
		static std::shared_ptr<VertexArrayBuffer> create(const GraphicContextPtr &gc, const void *data, int size, BufferUsage usage = usage_static_draw);

		/// \brief Uploads data to vertex array buffer.
		virtual void upload_data(const GraphicContextPtr &gc, int offset, const void *data, int size) = 0;

		/// \brief Copies data from transfer buffer
		virtual void copy_from(const GraphicContextPtr &gc, const TransferBufferPtr &buffer, int dest_pos = 0, int src_pos = 0, int size = -1) = 0;

		/// \brief Copies data to transfer buffer
		virtual void copy_to(const GraphicContextPtr &gc, const TransferBufferPtr &buffer, int dest_pos = 0, int src_pos = 0, int size = -1) = 0;
	};

	typedef std::shared_ptr<VertexArrayBuffer> VertexArrayBufferPtr;
}
