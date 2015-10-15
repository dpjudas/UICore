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

#include "UICore/Display/Render/vertex_array_buffer.h"
#include "UICore/Display/Render/graphic_context.h"
#include "UICore/GL/opengl.h"
#include "UICore/GL/gl_share_list.h"

namespace uicore
{
	class GraphicContext;
	class TransferBuffer;
	class GL3GraphicContext;

	class GL3BufferObject : GLSharedResource
	{
	public:
		GL3BufferObject();
		~GL3BufferObject();
		void create(const void *data, int size, BufferUsage usage, GLenum new_binding, GLenum new_target);

		void *get_data();
		const void *get_data() const;

		GLuint get_handle() const { return handle; }
		GLenum get_binding() const { return binding; }
		GLenum get_target() const { return target; }

		void lock(const GraphicContextPtr &gc, BufferAccess access);
		void unlock();
		void upload_data(const GraphicContextPtr &gc, int offset, const void *data, int size);

		void upload_data(const GraphicContextPtr &gc, const void *data, int size);
		void copy_from(const GraphicContextPtr &gc, const TransferBufferPtr &buffer, int dest_pos, int src_pos, int size);
		void copy_to(const GraphicContextPtr &gc, const TransferBufferPtr &buffer, int dest_pos, int src_pos, int size);

	private:
		void on_dispose() override;

		GLuint handle;
		GLenum binding;
		GLenum target;

		void *data_ptr;
		GraphicContextPtr lock_gc;
	};
}
