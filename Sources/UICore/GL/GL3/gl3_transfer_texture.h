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


#include "UICore/Display/Render/transfer_texture.h"
#include "UICore/GL/opengl.h"
#include "UICore/Core/System/disposable_object.h"
#include "gl3_buffer_object.h"

namespace uicore
{
	class GL3GraphicContext;

	class GL3TransferTexture : public TransferTexture
	{
	public:
		GL3TransferTexture(const void *data, const Size &new_size, PixelBufferDirection direction, TextureFormat new_format, BufferUsage usage);
		~GL3TransferTexture();

		void *data() override;
		const void *data() const override;
		int pitch() const override { return _pitch; }
		int width() const override { return _size.width; }
		int height() const override { return _size.height; }

		TextureFormat format() const override { return texture_format; };

		float pixel_ratio() const override { return _pixel_ratio; }
		void set_pixel_ratio(float ratio) override { _pixel_ratio = ratio; }

		GLuint get_handle() const { return buffer.get_handle(); }
		GLenum get_binding() const { return buffer.get_binding(); }
		GLenum get_target() const { return buffer.get_target(); }

		void lock(const GraphicContextPtr &gc, BufferAccess access) override { data_locked = true; buffer.lock(gc, access); }
		void unlock() override { data_locked = false; buffer.unlock(); }
		void upload_data(const GraphicContextPtr &gc, const Rect &dest_rect, const void *data) override;

	private:
		GL3BufferObject buffer;
		Size _size;
		TextureFormat texture_format;
		int _pitch;
		int _bytes_per_pixel;
		GLenum selected_binding;
		GLenum selected_target;
		bool data_locked;	// lock() has been called
		float _pixel_ratio = 0.0f;
	};
}
