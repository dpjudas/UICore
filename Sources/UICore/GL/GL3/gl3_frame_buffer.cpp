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
**    Harry Storbacka
*/

#include "UICore/precomp.h"
#include "UICore/Display/Render/render_buffer.h"
#include "UICore/Display/Render/texture_1d.h"
#include "UICore/Display/Render/texture_1d_array.h"
#include "UICore/Display/Render/texture_2d.h"
#include "UICore/Display/Render/texture_2d_array.h"
#include "UICore/Display/Render/texture_3d.h"
#include "UICore/Display/Render/texture_cube.h"
#include "UICore/Display/Render/texture_impl.h"
#include "UICore/Core/Text/string_format.h"
#include "UICore/Display/Render/texture.h"
#include "UICore/GL/opengl_wrap.h"
#include "UICore/GL/opengl.h"
#include "gl3_frame_buffer.h"
#include "gl3_render_buffer.h"
#include "gl3_texture_object.h"

namespace uicore
{
	GL3FrameBuffer::GL3FrameBuffer(GL3GraphicContext *gc_provider)
		: gc_provider(gc_provider)
	{
		gc_provider->add_disposable(this);

		OpenGL::set_active(gc_provider);

		if (glGenFramebuffers == nullptr)
			throw Exception("Framebuffer not supported on your graphics card.");

		glGenFramebuffers(1, &handle);

	}

	GL3FrameBuffer::~GL3FrameBuffer()
	{
		dispose();
	}

	void GL3FrameBuffer::on_dispose()
	{
		if (handle)
		{
			OpenGL::set_active(gc_provider);
			glDeleteFramebuffers(1, &handle);
			handle = 0;
		}

		// Detach all textures and renderbuffers
		for (int cnt = 0; cnt < num_attachment_offsets; cnt++)
		{
			attached_textures[cnt].reset();
			attached_renderbuffers[cnt].reset();
		}
		gc_provider->remove_disposable(this);
	}

	GLuint GL3FrameBuffer::get_handle()
	{
		return handle;
	}

	Size GL3FrameBuffer::get_size() const
	{
		bool size_set = false;
		Size size;

		// Find the minimum renderbuffer size
		for (int cnt = 0; cnt < num_attachment_offsets; cnt++)
		{
			if (attached_textures[cnt])
			{
				Size texture_size = std::dynamic_pointer_cast<Texture2D>(attached_textures[cnt])->size();
				if (size_set)
				{
					if (texture_size.width < size.width)
						size.width = texture_size.width;
					if (texture_size.height < size.height)
						size.height = texture_size.height;
				}
				else
				{
					size = texture_size;
					size_set = true;
				}
			}

			if (attached_renderbuffers[cnt])
			{
				Size texture_size = attached_renderbuffers[cnt]->size();
				if (size_set)
				{
					if (texture_size.width < size.width)
						size.width = texture_size.width;
					if (texture_size.height < size.height)
						size.height = texture_size.height;
				}
				else
				{
					size = texture_size;
					size_set = true;
				}
			}
		}

		return size;
	}

	FrameBufferBindTarget GL3FrameBuffer::get_bind_target() const
	{
		return bind_target;
	}

	void GL3FrameBuffer::attach_color(int attachment_index, const RenderBufferPtr &render_buffer)
	{
		FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
		bool replaced_object = attach_object(GL_COLOR_ATTACHMENT0 + attachment_index, render_buffer);

		if (!replaced_object)
		{
			count_color_attachments++;
		}
	}

	void GL3FrameBuffer::attach_color(int attachment_index, const Texture1DPtr &texture, int level)
	{
		FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
		bool replaced_object = attach_object(GL_COLOR_ATTACHMENT0 + attachment_index, texture, level, 0, 0);

		if (!replaced_object)
		{
			count_color_attachments++;
		}
	}

	void GL3FrameBuffer::attach_color(int attachment_index, const Texture1DArrayPtr &texture, int array_index, int level)
	{
		FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
		bool replaced_object = attach_object(GL_COLOR_ATTACHMENT0 + attachment_index, texture, level, array_index, 0);

		if (!replaced_object)
		{
			count_color_attachments++;
		}
	}

	void GL3FrameBuffer::attach_color(int attachment_index, const Texture2DPtr &texture, int level)
	{
		FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
		bool replaced_object = attach_object(GL_COLOR_ATTACHMENT0 + attachment_index, texture, level, 0, 0);

		if (!replaced_object)
		{
			count_color_attachments++;
		}
	}

	void GL3FrameBuffer::attach_color(int attachment_index, const Texture2DArrayPtr &texture, int array_index, int level)
	{
		FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
		bool replaced_object = attach_object(GL_COLOR_ATTACHMENT0 + attachment_index, texture, level, array_index, 0);

		if (!replaced_object)
		{
			count_color_attachments++;
		}
	}

	void GL3FrameBuffer::attach_color(int attachment_index, const Texture3DPtr &texture, int depth, int level)
	{
		FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
		bool replaced_object = attach_object(GL_COLOR_ATTACHMENT0 + attachment_index, texture, level, depth, 0);

		if (!replaced_object)
		{
			count_color_attachments++;
		}
	}

	void GL3FrameBuffer::attach_color(int attachment_index, const TextureCubePtr &texture, TextureSubtype subtype, int level)
	{
		FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
		bool replaced_object = attach_object(GL_COLOR_ATTACHMENT0 + attachment_index, texture, level, 0, decode_texture_subtype(subtype));

		if (!replaced_object)
		{
			count_color_attachments++;
		}
	}

	void GL3FrameBuffer::detach_color(int attachment_index)
	{
		FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
		detach_object(GL_COLOR_ATTACHMENT0 + attachment_index);

		count_color_attachments--;
	}

	void GL3FrameBuffer::attach_stencil(const RenderBufferPtr &render_buffer)
	{
		FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
		attach_object(GL_STENCIL_ATTACHMENT, render_buffer);
	}

	void GL3FrameBuffer::attach_stencil(const Texture2DPtr &texture, int level)
	{
		FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
		attach_object(GL_STENCIL_ATTACHMENT, texture, level, 0, 0);
	}

	void GL3FrameBuffer::attach_stencil(const TextureCubePtr &texture, TextureSubtype subtype, int level)
	{
		FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
		attach_object(GL_STENCIL_ATTACHMENT, texture, level, 0, decode_texture_subtype(subtype));
	}

	void GL3FrameBuffer::detach_stencil()
	{
		FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
		detach_object(GL_STENCIL_ATTACHMENT);
	}

	void GL3FrameBuffer::attach_depth(const RenderBufferPtr &render_buffer)
	{
		FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
		attach_object(GL_DEPTH_ATTACHMENT, render_buffer);
	}

	void GL3FrameBuffer::attach_depth(const Texture2DPtr &texture, int level)
	{
		FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
		attach_object(GL_DEPTH_ATTACHMENT, texture, level, 0, 0);
	}

	void GL3FrameBuffer::attach_depth(const TextureCubePtr &texture, TextureSubtype subtype, int level)
	{
		FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
		attach_object(GL_DEPTH_ATTACHMENT, texture, level, 0, decode_texture_subtype(subtype));
	}

	void GL3FrameBuffer::detach_depth()
	{
		FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
		detach_object(GL_DEPTH_ATTACHMENT);
	}

	void GL3FrameBuffer::attach_depth_stencil(const RenderBufferPtr &render_buffer)
	{
		FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
		attach_object(GL_DEPTH_STENCIL_ATTACHMENT, render_buffer);
	}

	void GL3FrameBuffer::attach_depth_stencil(const Texture2DPtr &texture, int level)
	{
		FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
		attach_object(GL_DEPTH_STENCIL_ATTACHMENT, texture, level, 0, 0);
	}

	void GL3FrameBuffer::attach_depth_stencil(const TextureCubePtr &texture, TextureSubtype subtype, int level)
	{
		FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
		attach_object(GL_DEPTH_STENCIL_ATTACHMENT, texture, level, 0, decode_texture_subtype(subtype));
	}

	void GL3FrameBuffer::detach_depth_stencil()
	{
		FrameBufferStateTracker tracker(bind_target, handle, gc_provider);
		detach_object(GL_DEPTH_STENCIL_ATTACHMENT);
	}

	void GL3FrameBuffer::set_bind_target(FrameBufferBindTarget target)
	{
		bind_target = target;
	}

	void GL3FrameBuffer::check_framebuffer_complete()
	{
		FrameBufferStateTracker tracker(bind_target, handle, gc_provider);

		int error_code = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (error_code != GL_FRAMEBUFFER_COMPLETE)
			throw Exception(string_format("FrameBuffer is : %1", get_error_message(error_code)));
	}

	void GL3FrameBuffer::bind_framebuffer(bool write_only)
	{
		glBindFramebuffer(write_only ? GL_FRAMEBUFFER : GL_READ_FRAMEBUFFER, handle);

		if (count_color_attachments)
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			glReadBuffer(GL_COLOR_ATTACHMENT0);
		}
		else
		{
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
	}

	std::string GL3FrameBuffer::get_error_message(int error_code)
	{
		switch (error_code)
		{
		case GL_FRAMEBUFFER_COMPLETE:
			return "FRAMEBUFFER_COMPLETE";
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			return "FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			return "FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			return "FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			return "FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
		case GL_FRAMEBUFFER_UNSUPPORTED:
			return "FRAMEBUFFER_UNSUPPORTED";
		default:
			return string_format("Error code: %1", error_code);
		}
	}

	FrameBufferStateTracker::FrameBufferStateTracker(FrameBufferBindTarget target, GLuint handle, GL3GraphicContext *gc_provider)
		: bind_target(target), last_bound(0), handle_and_bound_equal(false)
	{
		OpenGL::set_active(gc_provider);
		if (bind_target == framebuffer_draw)
		{
			glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &last_bound);
			if (last_bound == handle)
			{
				handle_and_bound_equal = true;
			}
			else
			{
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, handle);
			}
		}
		else
		{
			glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &last_bound);
			if (last_bound == handle)
			{
				handle_and_bound_equal = true;
			}
			else
			{
				glBindFramebuffer(GL_READ_FRAMEBUFFER, handle);
			}
		}
	}

	FrameBufferStateTracker::~FrameBufferStateTracker()
	{
		if (!handle_and_bound_equal)
		{
			if (bind_target == framebuffer_draw)
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, last_bound);
			else
				glBindFramebuffer(GL_READ_FRAMEBUFFER, last_bound);
		}
	}

	int GL3FrameBuffer::decode_internal_attachment_offset(GLenum opengl_attachment)
	{
		int internal_attachment_offset = 0;

		if (opengl_attachment == GL_STENCIL_ATTACHMENT)
		{
			internal_attachment_offset = stencil_attachment_offset;
		}
		else if (opengl_attachment == GL_DEPTH_ATTACHMENT)
		{
			internal_attachment_offset = depth_attachment_offset;
		}
		else if (opengl_attachment == GL_DEPTH_STENCIL_ATTACHMENT)
		{
			internal_attachment_offset = depth_stencil_attachment_offset;
		}
		else if ((opengl_attachment >= GL_COLOR_ATTACHMENT0) && (opengl_attachment < (GL_COLOR_ATTACHMENT0 + max_color_attachments)))
		{
			internal_attachment_offset = color_attachment_offset + (opengl_attachment - GL_COLOR_ATTACHMENT0);
		}
		else
		{
			throw Exception("Specified attachment index value is currently not supported");
		}
		return internal_attachment_offset;
	}


	bool GL3FrameBuffer::attach_object(GLenum opengl_attachment, const RenderBufferPtr &render_buffer)
	{
		int internal_attachment_offset = decode_internal_attachment_offset(opengl_attachment);

		// Check for object replacement
		bool is_replaced_object = false;
		if (attached_renderbuffers[internal_attachment_offset])
		{
			is_replaced_object = true;
			attached_renderbuffers[internal_attachment_offset].reset();
		}
		if (attached_textures[internal_attachment_offset])
		{
			is_replaced_object = true;
			attached_textures[internal_attachment_offset].reset();
		}

		// Store the renderbuffer
		attached_renderbuffers[internal_attachment_offset] = render_buffer;

		// Bind the renderbuffer
		GL3RenderBuffer *gl_render_buffer = static_cast<GL3RenderBuffer *>(render_buffer.get());
		if (!gl_render_buffer)
			throw Exception("Invalid render buffer");

		FrameBufferStateTracker tracker_draw(bind_target, handle, gc_provider);

		GLenum target = GL_DRAW_FRAMEBUFFER;
		if (bind_target == framebuffer_read)
			target = GL_READ_FRAMEBUFFER;

		GLuint render_buffer_handle = gl_render_buffer->get_handle();

		glFramebufferRenderbuffer(target, opengl_attachment, GL_RENDERBUFFER, render_buffer_handle);
		return is_replaced_object;
	}

	bool GL3FrameBuffer::attach_object(GLenum opengl_attachment, const TexturePtr &texture, int level, int zoffset, GLuint texture_target)
	{
		int internal_attachment_offset = decode_internal_attachment_offset(opengl_attachment);

		// Check for object replacement
		bool is_replaced_object = false;
		if (attached_renderbuffers[internal_attachment_offset])
		{
			is_replaced_object = true;
			attached_renderbuffers[internal_attachment_offset].reset();
		}
		if (attached_textures[internal_attachment_offset])
		{
			is_replaced_object = true;
			attached_textures[internal_attachment_offset] = TexturePtr();
		}

		// Store the texture
		attached_textures[internal_attachment_offset] = texture;

		// Bind the renderbuffer
		GL3TextureObject *gl_texture_provider = static_cast<GL3TextureObject*>(texture->texture_object());
		if (!gl_texture_provider)
			throw Exception("Invalid texture");

		GLuint texture_type = gl_texture_provider->get_texture_type();
		GLuint texture_handle = gl_texture_provider->get_handle();

		GLenum target = GL_DRAW_FRAMEBUFFER;
		if (bind_target == framebuffer_read)
			target = GL_READ_FRAMEBUFFER;

		if (!texture_target)
			texture_target = texture_type;

		if (texture_type == GL_TEXTURE_1D)
		{
			glFramebufferTexture1D(target, opengl_attachment, texture_target, texture_handle, level);
		}
		else if (texture_type == GL_TEXTURE_2D)
		{
			glFramebufferTexture2D(target, opengl_attachment, texture_target, texture_handle, level);
		}
		else if (texture_type == GL_TEXTURE_3D)
		{
			glFramebufferTexture3D(target, opengl_attachment, texture_target, texture_handle, level, zoffset);
		}
		else if (texture_type == GL_TEXTURE_2D_ARRAY || texture_type == GL_TEXTURE_1D_ARRAY)
		{
			glFramebufferTextureLayer(target, opengl_attachment, texture_handle, level, zoffset);
		}
		return is_replaced_object;
	}

	void GL3FrameBuffer::detach_object(GLenum opengl_attachment)
	{
		int internal_attachment_offset = decode_internal_attachment_offset(opengl_attachment);

		GLenum target = GL_DRAW_FRAMEBUFFER;
		if (bind_target == framebuffer_read)
			target = GL_READ_FRAMEBUFFER;

		if (attached_renderbuffers[internal_attachment_offset])
		{
			glFramebufferRenderbuffer(target, opengl_attachment, GL_RENDERBUFFER, 0);
			attached_renderbuffers[internal_attachment_offset].reset();
		}

		if (attached_textures[internal_attachment_offset])
		{
			GL3TextureObject *gl_texture_provider = static_cast<GL3TextureObject*>(attached_textures[internal_attachment_offset]->texture_object());
			GLuint texture_type = gl_texture_provider->get_texture_type();

			if (texture_type == GL_TEXTURE_1D)
			{
				glFramebufferTexture1D(target, opengl_attachment, texture_type, 0, 0);
			}
			else if (texture_type == GL_TEXTURE_2D)
			{
				glFramebufferTexture2D(target, opengl_attachment, texture_type, 0, 0);
			}
			else if (texture_type == GL_TEXTURE_3D)
			{
				glFramebufferTexture3D(target, opengl_attachment, texture_type, 0, 0, 0);
			}
			else if (texture_type == GL_TEXTURE_2D_ARRAY || texture_type == GL_TEXTURE_1D_ARRAY)
			{
				glFramebufferTextureLayer(target, opengl_attachment, 0, 0, 0);
			}

			attached_textures[internal_attachment_offset].reset();
		}
	}

	GLuint GL3FrameBuffer::decode_texture_subtype(TextureSubtype subtype)
	{
		GLuint texture_target;
		switch (subtype)
		{
		case subtype_cube_map_positive_x:
			texture_target = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
			break;
		case subtype_cube_map_negative_x:
			texture_target = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
			break;
		case subtype_cube_map_positive_y:
			texture_target = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
			break;
		case subtype_cube_map_negative_y:
			texture_target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
			break;
		case subtype_cube_map_positive_z:
			texture_target = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
			break;
		case subtype_cube_map_negative_z:
			texture_target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
			break;
		default:
			texture_target = 0;
		}
		return texture_target;
	}
}
