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
**    Harry Storbacka
**    Ken Hirsch
**    Kenneth Gangstoe
**    Mark Page
**    Kevin J Bluck
*/

#include "UICore/precomp.h"
#include "gl3_graphic_context.h"
#include "gl3_occlusion_query.h"
#include "gl3_texture_object.h"
#include "gl3_program_object.h"
#include "gl3_shader_object.h"
#include "gl3_frame_buffer.h"
#include "gl3_render_buffer.h"
#include "gl3_vertex_array_buffer.h"
#include "gl3_uniform_buffer.h"
#include "gl3_storage_buffer.h"
#include "gl3_element_array_buffer.h"
#include "gl3_transfer_buffer.h"
#include "gl3_transfer_texture.h"
#include "gl3_primitives_array.h"
#include "UICore/Core/System/databuffer.h"
#include "UICore/Core/Math/cl_math.h"
#include "UICore/Core/Math/vec3.h"
#include "UICore/Core/Text/text.h"
#include "UICore/Core/Text/string_format.h"
#include "UICore/Display/Render/texture.h"
#include "UICore/Display/Render/primitives_array.h"
#include "UICore/Display/Image/pixel_buffer.h"
#include "UICore/Display/Render/frame_buffer.h"
#include "UICore/Display/Render/program_object.h"
#include "UICore/Display/Window/display_window_provider.h"
#include "UICore/Display/Render/texture_provider.h"
#include "UICore/GL/gl_share_list.h"
#include "UICore/GL/opengl.h"
#include "UICore/GL/opengl_wrap.h"
#include "UICore/Display/2D/image.h"
#include "UICore/GL/opengl_context_description.h"
#ifdef __APPLE__
#include "../Platform/OSX/opengl_window_provider_osx.h"
#elif !defined(WIN32)

#if defined(__ANDROID__)
#include "../Platform/Android/opengl_window_provider_android.h"
#else
#include "../Platform/GLX/opengl_window_provider_glx.h"
#endif
#endif

#ifdef WIN32
#include "../Platform/WGL/opengl_window_provider_wgl.h"
#endif

#include <memory>

namespace uicore
{
	GL3GraphicContext::GL3GraphicContext(const OpenGLWindowProvider * const render_window)
		: render_window(render_window), framebuffer_bound(false), opengl_version_major(0), shader_version_major(0), scissor_enabled(false)
	{
		check_opengl_version();
		calculate_shading_language_version();

		create_standard_programs();

		reset_program_object();

		GLShareList::context_created(this);

		set_default_state();
	}

	GL3GraphicContext::~GL3GraphicContext()
	{
		dispose();
	}

	void GL3GraphicContext::create_standard_programs()
	{
		for (GraphicContextImpl *gc : GLShareList::all_contexts())
		{
			GL3GraphicContext* gc_provider = dynamic_cast<GL3GraphicContext *>(gc);
			if (gc_provider != this)
			{
				standard_programs = gc_provider->standard_programs;
				return;
			}
		}

		standard_programs = GL3StandardPrograms(this);

	}

	void GL3GraphicContext::on_dispose()
	{
		while (!disposable_objects.empty())
			disposable_objects.front()->dispose();

		standard_programs = GL3StandardPrograms();

		GLShareList::context_destroyed(this);
		OpenGL::remove_active(this);
	}

	void GL3GraphicContext::add_disposable(DisposableObject *disposable)
	{
		disposable_objects.push_back(disposable);
	}

	void GL3GraphicContext::remove_disposable(DisposableObject *disposable)
	{
		for (size_t i = 0; i < disposable_objects.size(); i++)
		{
			if (disposable_objects[i] == disposable)
			{
				disposable_objects.erase(disposable_objects.begin() + i);
				return;
			}
		}
	}

	ProcAddress *GL3GraphicContext::get_proc_address(const std::string& function_name) const
	{
		return render_window->get_proc_address(function_name);
	}

	void GL3GraphicContext::check_opengl_version()
	{
		int version_major = 0;
		int version_minor = 0;

		get_opengl_version(version_major, version_minor);
		if (version_major < 3)
			throw Exception(string_format("This application requires OpenGL 3.0 or above. Your hardware only supports OpenGL %1.%2. Try updating your drivers, or upgrade to a newer graphics card.", version_major, version_minor));
	}

	void GL3GraphicContext::get_opengl_version(int &version_major, int &version_minor) const
	{
		if (!opengl_version_major)	// Is not cached
		{

			/*	The GL_VERSION string begins with a version number. The version number uses one of these forms:
				major_number.minor_number
				major_number.minor_number.release_number
				Vendor-specific information may follow the version number. Its format depends on the implementation, but a space always separates the version number and the vendor-specific information.
				All strings are null-terminated.
				If an error is generated, glGetString returns zero.
				*/
			OpenGL::set_active(this);

#if defined(__IOS__)
			opengl_version_major = 0;
			opengl_version_minor = 0;
			glGetIntegerv(GL_MAJOR_VERSION, &opengl_version_major);
			glGetIntegerv(GL_MINOR_VERSION, &opengl_version_minor);
#else

			std::string version = (char*)glGetString(GL_VERSION);

			opengl_version_major = 0;
			opengl_version_minor = 0;

			std::vector<std::string> split_version = Text::split(version, ".");
			if (split_version.size() > 0)
				opengl_version_major = Text::parse_int32(split_version[0]);
			if (split_version.size() > 1)
				opengl_version_minor = Text::parse_int32(split_version[1]);
#endif
		}

		version_major = opengl_version_major;
		version_minor = opengl_version_minor;
	}

	void GL3GraphicContext::calculate_shading_language_version()
	{
		// See http://www.opengl.org/wiki/Detecting_the_Shader_Model
		shader_version_major = 0;
		shader_version_minor = 0;

		if ((opengl_version_major < 2) || ((opengl_version_major == 2) && (opengl_version_minor < 1)))
		{
			OpenGL::set_active(this);

			std::string version = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

			std::vector<std::string> split_version = Text::split(version, ".");
			if (split_version.size() > 0)
				shader_version_major = Text::parse_int32(split_version[0]);
			if (split_version.size() > 1)
				shader_version_minor = Text::parse_int32(split_version[1]);
		}
		else
		{
			if (opengl_version_major == 2)
			{
				shader_version_major = 1;
				shader_version_minor = 20;
			}
			else if ((opengl_version_major == 3) && (opengl_version_minor == 0))
			{
				shader_version_major = 1;
				shader_version_minor = 30;
			}
			else if ((opengl_version_major == 3) && (opengl_version_minor == 1))
			{
				shader_version_major = 1;
				shader_version_minor = 40;
			}
			else if ((opengl_version_major == 3) && (opengl_version_minor == 2))
			{
				shader_version_major = 1;
				shader_version_minor = 50;
			}
			else
			{
				shader_version_major = opengl_version_major;
				shader_version_minor = opengl_version_minor * 10;
			}
		}
	}

	int GL3GraphicContext::max_attributes()
	{
		OpenGL::set_active(this);
		GLint max_attributes = 0;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_attributes);
		if (max_attributes < 16)
			max_attributes = 16;
		return max_attributes;
	}

	Size GL3GraphicContext::max_texture_size() const
	{
		OpenGL::set_active(this);
		GLint max_size = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_size);
		return Size(max_size, max_size);
	}

	Size GL3GraphicContext::display_window_size() const
	{
		return render_window->backing_viewport().get_size();
	}

	float GL3GraphicContext::pixel_ratio() const
	{
		return render_window->pixel_ratio();
	}

	std::shared_ptr<RenderBuffer> GL3GraphicContext::create_render_buffer(int width, int height, TextureFormat texture_format, int multisample_samples)
	{
		return std::make_shared<GL3RenderBuffer>(width, height, texture_format, multisample_samples);
	}

	std::shared_ptr<RasterizerState> GL3GraphicContext::create_rasterizer_state(const RasterizerStateDescription &desc)
	{
		auto it = rasterizer_states.find(desc);
		if (it != rasterizer_states.end())
		{
			return it->second;
		}
		else
		{
			auto state = std::make_shared<OpenGLRasterizerState>(desc);
			rasterizer_states[desc.clone()] = state;
			return state;
		}
	}

	std::shared_ptr<BlendState> GL3GraphicContext::create_blend_state(const BlendStateDescription &desc)
	{
		auto it = blend_states.find(desc);
		if (it != blend_states.end())
		{
			return it->second;
		}
		else
		{
			auto state = std::make_shared<OpenGLBlendState>(desc);
			blend_states[desc.clone()] = state;
			return state;
		}
	}

	std::shared_ptr<DepthStencilState> GL3GraphicContext::create_depth_stencil_state(const DepthStencilStateDescription &desc)
	{
		auto it = depth_stencil_states.find(desc);
		if (it != depth_stencil_states.end())
		{
			return it->second;
		}
		else
		{
			auto state = std::make_shared<OpenGLDepthStencilState>(desc);
			depth_stencil_states[desc.clone()] = state;
			return state;
		}
	}

	std::shared_ptr<ShaderObject> GL3GraphicContext::create_shader(ShaderType type, const std::string &source)
	{
		return std::make_shared<GL3ShaderObject>(type, source);
	}

	std::shared_ptr<ShaderObject> GL3GraphicContext::create_shader(ShaderType type, const void *bytecode, int bytecode_size)
	{
		throw Exception("Shader Objects with bytecode are not supported for OpenGL");
	}

	std::shared_ptr<ProgramObject> GL3GraphicContext::create_program()
	{
		return std::make_shared<GL3ProgramObject>();
	}

	std::shared_ptr<OcclusionQuery> GL3GraphicContext::create_occlusion_query()
	{
		return std::make_shared<GL3OcclusionQuery>(this);
	}

	std::shared_ptr<FrameBuffer> GL3GraphicContext::create_frame_buffer()
	{
		return std::make_shared<GL3FrameBuffer>(this);
	}

	std::shared_ptr<StorageBuffer> GL3GraphicContext::create_storage_buffer(int size, int stride, BufferUsage usage)
	{
		return std::make_shared<GL3StorageBuffer>(size, stride, usage);
	}

	std::shared_ptr<StorageBuffer> GL3GraphicContext::create_storage_buffer(const void *data, int size, int stride, BufferUsage usage)
	{
		return std::make_shared<GL3StorageBuffer>(data, size, stride, usage);
	}

	std::shared_ptr<ElementArrayBuffer> GL3GraphicContext::create_element_array_buffer(int size, BufferUsage usage)
	{
		return std::make_shared<GL3ElementArrayBuffer>(size, usage);
	}

	std::shared_ptr<ElementArrayBuffer> GL3GraphicContext::create_element_array_buffer(const void *data, int size, BufferUsage usage)
	{
		return std::make_shared<GL3ElementArrayBuffer>(data, size, usage);
	}

	std::shared_ptr<VertexArrayBuffer> GL3GraphicContext::create_vertex_array_buffer(int size, BufferUsage usage)
	{
		return std::make_shared<GL3VertexArrayBuffer>(size, usage);
	}

	std::shared_ptr<VertexArrayBuffer> GL3GraphicContext::create_vertex_array_buffer(const void *data, int size, BufferUsage usage)
	{
		return std::make_shared<GL3VertexArrayBuffer>(data, size, usage);
	}

	std::shared_ptr<UniformBuffer> GL3GraphicContext::create_uniform_buffer(int size, BufferUsage usage)
	{
		return std::make_shared<GL3UniformBuffer>(size, usage);
	}

	std::shared_ptr<UniformBuffer> GL3GraphicContext::create_uniform_buffer(const void *data, int size, BufferUsage usage)
	{
		return std::make_shared<GL3UniformBuffer>(data, size, usage);
	}

	std::shared_ptr<TransferBuffer> GL3GraphicContext::create_transfer_buffer(int size, BufferUsage usage)
	{
		return std::make_shared<GL3TransferBuffer>(size, usage);
	}

	std::shared_ptr<TransferBuffer> GL3GraphicContext::create_transfer_buffer(const void *data, int size, BufferUsage usage)
	{
		return std::make_shared<GL3TransferBuffer>(data, size, usage);
	}

	std::shared_ptr<PrimitivesArray> GL3GraphicContext::create_primitives_array()
	{
		return std::make_shared<GL3PrimitivesArray>(this);
	}

	std::shared_ptr<Texture1D> GL3GraphicContext::create_texture_1d(int width, TextureFormat texture_format, int levels)
	{
		return std::make_shared<Texture1DImpl<GL3TextureObject>>(GL3TextureObject::InitData(), width, texture_format, levels);
	}

	std::shared_ptr<Texture1DArray> GL3GraphicContext::create_texture_1d_array(int width, int array_size, TextureFormat texture_format, int levels)
	{
		return std::make_shared<Texture1DArrayImpl<GL3TextureObject>>(GL3TextureObject::InitData(), width, array_size, texture_format, levels);
	}

	std::shared_ptr<Texture2D> GL3GraphicContext::create_texture_2d(int width, int height, TextureFormat texture_format, int levels)
	{
		return std::make_shared<Texture2DImpl<GL3TextureObject>>(GL3TextureObject::InitData(), width, height, texture_format, levels);
	}

	std::shared_ptr<Texture2DArray> GL3GraphicContext::create_texture_2d_array(int width, int height, int array_size, TextureFormat texture_format, int levels)
	{
		return std::make_shared<Texture2DArrayImpl<GL3TextureObject>>(GL3TextureObject::InitData(), width, height, array_size, texture_format, levels);
	}

	std::shared_ptr<Texture3D> GL3GraphicContext::create_texture_3d(int width, int height, int depth, TextureFormat texture_format, int levels)
	{
		return std::make_shared<Texture3DImpl<GL3TextureObject>>(GL3TextureObject::InitData(), width, height, depth, texture_format, levels);
	}

	std::shared_ptr<TextureCube> GL3GraphicContext::create_texture_cube(int width, int height, TextureFormat texture_format, int levels)
	{
		return std::make_shared<TextureCubeImpl<GL3TextureObject>>(GL3TextureObject::InitData(), width, height, texture_format, levels);
	}

	std::shared_ptr<TextureCubeArray> GL3GraphicContext::create_texture_cube_array(int width, int height, int array_size, TextureFormat texture_format, int levels)
	{
		return std::make_shared<TextureCubeArrayImpl<GL3TextureObject>>(GL3TextureObject::InitData(), width, height, array_size, texture_format, levels);
	}

	std::shared_ptr<TransferTexture> GL3GraphicContext::create_transfer_texture(const void *data, const Size &size, PixelBufferDirection direction, TextureFormat format, BufferUsage usage)
	{
		return std::make_shared<GL3TransferTexture>(data, size, direction, format, usage);
	}

	void GL3GraphicContext::set_rasterizer_state(const RasterizerStatePtr &state)
	{
		if (state)
		{
			OpenGLRasterizerState *gl3_state = static_cast<OpenGLRasterizerState*>(state.get());
			if (gl3_state)
			{
				selected_rasterizer_state.set(gl3_state->desc);
				OpenGL::set_active(this);
				selected_rasterizer_state.apply();
				scissor_enabled = gl3_state->desc.get_enable_scissor();
			}
		}
		else
		{
			set_rasterizer_state(default_rasterizer_state());
		}
	}

	void GL3GraphicContext::set_blend_state(const BlendStatePtr &state, const Colorf &blend_color, unsigned int sample_mask)
	{
		if (state)
		{
			OpenGLBlendState *gl3_state = static_cast<OpenGLBlendState*>(state.get());
			if (gl3_state)
			{
				selected_blend_state.set(gl3_state->desc, blend_color);
				OpenGL::set_active(this);
				selected_blend_state.apply();
			}
		}
		else
		{
			set_blend_state(default_blend_state(), blend_color, sample_mask);
		}
	}

	void GL3GraphicContext::set_depth_stencil_state(const DepthStencilStatePtr &state, int stencil_ref)
	{
		if (state)
		{
			OpenGLDepthStencilState *gl3_state = static_cast<OpenGLDepthStencilState*>(state.get());
			if (gl3_state)
			{
				selected_depth_stencil_state.set(gl3_state->desc);
				OpenGL::set_active(this);
				selected_depth_stencil_state.apply();
			}
		}
		else
		{
			set_depth_stencil_state(default_depth_stencil_state(), stencil_ref);
		}
	}

	std::shared_ptr<PixelBuffer> GL3GraphicContext::pixeldata(const Rect& rect, TextureFormat texture_format, bool clamp) const
	{
		TextureFormat_GL tf = OpenGL::get_textureformat(texture_format);
		if (!tf.valid)
			throw Exception("Unsupported texture format passed to GraphicContext::get_pixeldata");

		auto pbuf = PixelBuffer::create(rect.get_width(), rect.get_height(), texture_format);
		OpenGL::set_active(this);
		if (!framebuffer_bound)
		{
			render_window->is_double_buffered() ? glReadBuffer(GL_BACK) : glReadBuffer(GL_FRONT);
		}
		if (glClampColor)
			glClampColor(GL_CLAMP_READ_COLOR, clamp ? GL_TRUE : GL_FALSE);

		Size display_size = display_window_size();

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_PACK_ROW_LENGTH, pbuf->pitch() / pbuf->bytes_per_pixel());
		glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_PACK_SKIP_ROWS, 0);
		glReadPixels(rect.left, display_size.height - rect.bottom, rect.get_width(), rect.get_height(), tf.pixel_format, tf.pixel_datatype, pbuf->data());
		pbuf->flip_vertical();
		return pbuf;
	}

	void GL3GraphicContext::set_uniform_buffer(int index, const UniformBufferPtr &buffer)
	{
		if (buffer)
		{
			OpenGL::set_active(this);
			glBindBufferBase(GL_UNIFORM_BUFFER, index, static_cast<GL3UniformBuffer*>(buffer.get())->get_handle());
		}
		else
		{
			OpenGL::set_active(this);
			glBindBufferBase(GL_UNIFORM_BUFFER, index, 0);
		}
	}

	void GL3GraphicContext::set_storage_buffer(int index, const StorageBufferPtr &buffer)
	{
		if (buffer)
		{
			OpenGL::set_active(this);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, static_cast<GL3StorageBuffer*>(buffer.get())->get_handle());
		}
		else
		{
			OpenGL::set_active(this);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, 0);
		}
	}

	void GL3GraphicContext::set_texture(int unit_index, const TexturePtr &texture)
	{
		if (texture)
		{
			OpenGL::set_active(this);

			if (glActiveTexture != nullptr)
			{
				glActiveTexture(GL_TEXTURE0 + unit_index);
			}
			else if (unit_index > 0)
			{
				return;
			}

			if (texture)
			{
				GL3TextureObject *provider = static_cast<GL3TextureObject*>(texture->texture_object());
				glBindTexture(provider->get_texture_type(), provider->get_handle());
			}
		}
		else
		{
			OpenGL::set_active(this);

			if (glActiveTexture != nullptr)
			{
				glActiveTexture(GL_TEXTURE0 + unit_index);
			}
			else if (unit_index > 0)
			{
				return;
			}
			// Set the texture to the default state
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void GL3GraphicContext::set_image_texture(int unit_index, const TexturePtr &texture)
	{
		if (texture)
		{
			OpenGL::set_active(this);
			GL3TextureObject *provider = static_cast<GL3TextureObject*>(texture->texture_object());
			glBindImageTexture(unit_index, provider->get_handle(), 0, GL_FALSE, 0, GL_READ_WRITE, provider->get_internal_format());
		}
		else
		{
			OpenGL::set_active(this);
			glBindImageTexture(unit_index, 0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
		}
	}

	bool GL3GraphicContext::is_frame_buffer_owner(const FrameBufferPtr &fb)
	{
		GL3FrameBuffer *fb_provider = dynamic_cast<GL3FrameBuffer *>(fb.get());
		if (fb_provider)
			return fb_provider->get_gc_provider() == this;
		else
			return false;
	}

	void GL3GraphicContext::set_frame_buffer(const FrameBufferPtr &draw_buffer, const FrameBufferPtr &read_buffer)
	{
		_write_frame_buffer = draw_buffer;
		_read_frame_buffer = read_buffer;

		if (draw_buffer && read_buffer)
		{
			GL3FrameBuffer *draw_buffer_provider = dynamic_cast<GL3FrameBuffer *>(draw_buffer.get());
			GL3FrameBuffer *read_buffer_provider = dynamic_cast<GL3FrameBuffer *>(read_buffer.get());

			if (draw_buffer_provider->get_gc_provider() != this || read_buffer_provider->get_gc_provider() != this)
				throw Exception("FrameBuffer objects cannot be shared between multiple GraphicContext objects");

			OpenGL::set_active(this);

			draw_buffer_provider->bind_framebuffer(true);
			if (draw_buffer_provider != read_buffer_provider)		// You cannot read and write to the same framebuffer
				read_buffer_provider->bind_framebuffer(false);

			// Check for framebuffer completeness
			draw_buffer_provider->check_framebuffer_complete();
			if (draw_buffer_provider != read_buffer_provider)
				read_buffer_provider->check_framebuffer_complete();

			framebuffer_bound = true;
		}
		else
		{
			OpenGL::set_active(this);

			// To do: move this to OpenGLWindowProvider abstraction (some targets doesn't have a default frame buffer)
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

			if (render_window->is_double_buffered())
			{
				glDrawBuffer(GL_BACK);
				glReadBuffer(GL_BACK);
			}
			else
			{
				glDrawBuffer(GL_FRONT);
				glReadBuffer(GL_FRONT);
			}

			framebuffer_bound = false;
		}
	}

	void GL3GraphicContext::set_program_object(StandardProgram standard_program)
	{
		set_program_object(standard_programs.get_program_object(standard_program));
	}

	void GL3GraphicContext::set_program_object(const ProgramObjectPtr &program)
	{
		_program_object = program;

		OpenGL::set_active(this);
		if (glUseProgram == nullptr)
			return;

		if (program)
		{
			glUseProgram(static_cast<GL3ProgramObject*>(program.get())->get_handle());
		}
		else
		{
			glUseProgram(0);
		}
	}

	bool GL3GraphicContext::is_primitives_array_owner(const PrimitivesArrayPtr &prim_array)
	{
		GL3PrimitivesArray *prim_array_provider = dynamic_cast<GL3PrimitivesArray *>(prim_array.get());
		if (prim_array_provider)
			return prim_array_provider->get_gc_provider() == this;
		else
			return false;
	}

	void GL3GraphicContext::draw_primitives(PrimitivesType type, int num_vertices, const PrimitivesArrayPtr &primitives_array)
	{
		set_primitives_array(primitives_array);
		draw_primitives_array(type, 0, num_vertices);
		reset_primitives_array();
	}

	void GL3GraphicContext::set_primitives_array(const PrimitivesArrayPtr &primitives_array)
	{
		if (primitives_array)
		{
			GL3PrimitivesArray *prim_array = static_cast<GL3PrimitivesArray *>(primitives_array.get());

			OpenGL::set_active(this);
			glBindVertexArray(prim_array->handle);
		}
		else
		{
			OpenGL::set_active(this);
			glBindVertexArray(0);
		}
	}

	void GL3GraphicContext::draw_primitives_array(PrimitivesType type, int offset, int num_vertices)
	{
		OpenGL::set_active(this);
		glDrawArrays(OpenGL::to_enum(type), offset, num_vertices);
	}

	void GL3GraphicContext::draw_primitives_array_instanced(PrimitivesType type, int offset, int num_vertices, int instance_count)
	{
		OpenGL::set_active(this);
		glDrawArraysInstanced(OpenGL::to_enum(type), offset, num_vertices, instance_count);
	}

	void GL3GraphicContext::set_primitives_elements(const ElementArrayBufferPtr &array_provider)
	{
		if (array_provider)
		{
			OpenGL::set_active(this);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<GL3ElementArrayBuffer *>(array_provider.get())->get_handle());
		}
		else
		{
			OpenGL::set_active(this);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}

	void GL3GraphicContext::draw_primitives_elements(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset)
	{
		OpenGL::set_active(this);
		glDrawElements(OpenGL::to_enum(type), count, OpenGL::to_enum(indices_type), (const GLvoid*)offset);
	}

	void GL3GraphicContext::draw_primitives_elements_instanced(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset, int instance_count)
	{
		OpenGL::set_active(this);
		glDrawElementsInstanced(OpenGL::to_enum(type), count, OpenGL::to_enum(indices_type), (const GLvoid*)offset, instance_count);
	}

	void GL3GraphicContext::draw_primitives_elements(
		PrimitivesType type,
		int count,
		const ElementArrayBufferPtr &array_provider,
		VertexAttributeDataType indices_type,
		size_t offset)
	{
		OpenGL::set_active(this);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<GL3ElementArrayBuffer *>(array_provider.get())->get_handle());
		glDrawElements(OpenGL::to_enum(type), count, OpenGL::to_enum(indices_type), reinterpret_cast<void*>(offset));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void GL3GraphicContext::draw_primitives_elements_instanced(
		PrimitivesType type,
		int count,
		const ElementArrayBufferPtr &array_provider,
		VertexAttributeDataType indices_type,
		size_t offset,
		int instance_count)
	{
		OpenGL::set_active(this);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<GL3ElementArrayBuffer *>(array_provider.get())->get_handle());
		glDrawElementsInstanced(OpenGL::to_enum(type), count, OpenGL::to_enum(indices_type), reinterpret_cast<void*>(offset), instance_count);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void GL3GraphicContext::set_scissor(const Rect &rect)
	{
		OpenGL::set_active(this);

		if (!scissor_enabled)
			throw Exception("RasterizerState must be set with enable_scissor() for clipping to work");

		glEnable(GL_SCISSOR_TEST);
		glScissor(
			rect.left,
			rect.top,
			rect.get_width(),
			rect.get_height());
	}

	void GL3GraphicContext::reset_scissor()
	{
		OpenGL::set_active(this);
		glDisable(GL_SCISSOR_TEST);
	}

	void GL3GraphicContext::dispatch(int x, int y, int z)
	{
		OpenGL::set_active(this);
		glDispatchCompute(x, y, z);
	}

	void GL3GraphicContext::clear(const Colorf &color)
	{
		OpenGL::set_active(this);
		glClearColor(
			(GLclampf)color.r,
			(GLclampf)color.g,
			(GLclampf)color.b,
			(GLclampf)color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GL3GraphicContext::clear_stencil(int value)
	{
		OpenGL::set_active(this);
		glClearStencil(value);
		glClear(GL_STENCIL_BUFFER_BIT);
	}

	void GL3GraphicContext::clear_depth(float value)
	{
		OpenGL::set_active(this);
		if (glClearDepth)
			glClearDepth(value);
		else
			glClearDepthf(value);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void GL3GraphicContext::on_window_resized()
	{
		window_resized_signal(render_window->backing_viewport().get_size());
	}

	void GL3GraphicContext::set_viewport(const Rectf &viewport)
	{
		OpenGL::set_active(this);
		glViewport(
			GLsizei(viewport.left),
			GLsizei(viewport.top),
			GLsizei(viewport.right - viewport.left),
			GLsizei(viewport.bottom - viewport.top));
	}

	void GL3GraphicContext::set_viewport(int index, const Rectf &viewport)
	{
		if (glViewportIndexedf)
		{
			OpenGL::set_active(this);
			glViewportIndexedf(index,
				GLfloat(viewport.left),
				GLfloat(viewport.top),
				GLfloat(viewport.right - viewport.left),
				GLfloat(viewport.bottom - viewport.top));
		}
		else
		{
			if (index == 0)
				set_viewport(viewport);
		}
	}

	void GL3GraphicContext::set_depth_range(int viewport, float n, float f)
	{
		if (viewport == -1)
		{
			OpenGL::set_active(this);
			glDepthRange((double)n, (double)f); // glDepthRangef is from the OpenGL 4.1 extension ARB_ES2_Compatibility.
		}
		else
		{
			OpenGL::set_active(this);
			glDepthRangeIndexed(viewport, (float)n, (float)f);
		}
	}

	void GL3GraphicContext::set_draw_buffer(DrawBuffer buffer)
	{
		OpenGL::set_active(this);

		if (!render_window->is_double_buffered())	// Silently fix incorrect render buffers
		{
			if (buffer == buffer_back)
				buffer = buffer_front;
		}

		if (glDrawBuffer)
			glDrawBuffer(OpenGL::to_enum(buffer));

	}

	void GL3GraphicContext::make_current() const
	{
		render_window->make_current();
	}

	void GL3GraphicContext::flush()
	{
		OpenGL::set_active(this);
		glFlush();
	}

	const DisplayWindowProvider & GL3GraphicContext::get_render_window() const
	{
		return *render_window;
	}
}
