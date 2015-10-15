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
**    Mark Page
*/

#pragma once

#include <memory>
#include "graphic_context.h"
#include "../Image/image_import_description.h"
#include "../Image/texture_format.h"
#include "../Image/pixel_buffer.h"
#include "buffer_usage.h"

namespace uicore
{
	class Color;
	class Point;
	class PixelBuffer;
	class PixelFormat;

	/// \brief Pixel buffer prefered direction
	enum class PixelBufferDirection : int
	{
		/// \brief Use of the pixel buffer is to send data to the gpu
		data_to_gpu,

		/// \brief Use of the pixel buffer is to retrieve data from the gpu
		data_from_gpu
	};

	/// \brief Texture Transfer class.
	class TransferTexture : public PixelBuffer
	{
	public:
		/// \brief Constructs a GPU Transfer Buffer
		///
		/// \param gc = Graphic Context
		/// \param width = value
		/// \param height = value
		/// \param direction = prefered direction that you will use tranferring the pixel buffer data
		/// \param sized_format = Texture Format
		/// \param data = data to copy from (May be nullptr)
		/// \param usage = Buffer Usage
		/// \param pbuff = The pixelbuffer to copy from
		static std::shared_ptr<TransferTexture> create(const GraphicContextPtr &gc, int width, int height, PixelBufferDirection direction = data_to_gpu, TextureFormat texture_format = tf_rgba8, const void *data = nullptr, BufferUsage usage = usage_stream_draw);
		static std::shared_ptr<TransferTexture> create(const GraphicContextPtr &gc, const PixelBufferPtr &pbuff, PixelBufferDirection direction = data_to_gpu, BufferUsage usage = usage_stream_draw);

		/// \brief Maps buffer into system memory.
		///
		/// Locking before accessing data is only required for GPU based buffers.
		virtual void lock(const GraphicContextPtr &gc, BufferAccess access) = 0;

		/// \brief Unmaps buffer.
		virtual void unlock() = 0;

		/// \brief Uploads data to buffer.
		virtual void upload_data(const GraphicContextPtr &gc, const Rect &dest_rect, const void *data) = 0;
	};

	typedef std::shared_ptr<TransferTexture> TransferTexturePtr;
}
