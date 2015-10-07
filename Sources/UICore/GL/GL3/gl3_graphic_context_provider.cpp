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
#include "gl3_graphic_context_provider.h"
#include "gl3_occlusion_query_provider.h"
#include "gl3_texture_provider.h"
#include "gl3_program_object_provider.h"
#include "gl3_shader_object_provider.h"
#include "gl3_frame_buffer_provider.h"
#include "gl3_render_buffer_provider.h"
#include "gl3_vertex_array_buffer_provider.h"
#include "gl3_uniform_buffer_provider.h"
#include "gl3_storage_buffer_provider.h"
#include "gl3_element_array_buffer_provider.h"
#include "gl3_transfer_buffer_provider.h"
#include "gl3_pixel_buffer_provider.h"
#include "gl3_primitives_array_provider.h"
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
#include "UICore/Display/TargetProviders/display_window_provider.h"
#include "UICore/Display/TargetProviders/texture_provider.h"
#include "UICore/Display/Render/shared_gc_data.h"
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
	GL3GraphicContextProvider::GL3GraphicContextProvider(const OpenGLWindowProvider * const render_window)
		: render_window(render_window), framebuffer_bound(false), opengl_version_major(0), shader_version_major(0), scissor_enabled(false)
	{
		check_opengl_version();
		calculate_shading_language_version();

		create_standard_programs();

		reset_program_object();

		SharedGCData::add_provider(this);
	}

	GL3GraphicContextProvider::~GL3GraphicContextProvider()
	{
		dispose();
	}

	void GL3GraphicContextProvider::create_standard_programs()
	{
		// Find an existing provider
		std::unique_ptr<std::unique_lock<std::recursive_mutex>> mutex_section;
		std::vector<GraphicContextProvider*> &gc_providers = SharedGCData::get_gc_providers(mutex_section);

		unsigned int max = gc_providers.size();
		for (unsigned int cnt = 0; cnt < max; cnt++)
		{
			GL3GraphicContextProvider* gc_provider = dynamic_cast<GL3GraphicContextProvider *>(gc_providers[cnt]);
			if (gc_provider != this)
			{
				standard_programs = gc_provider->standard_programs;
				return;
			}
		}

		standard_programs = GL3StandardPrograms(this);

	}
	void GL3GraphicContextProvider::on_dispose()
	{
		while (!disposable_objects.empty())
			disposable_objects.front()->dispose();

		standard_programs = GL3StandardPrograms();

		SharedGCData::remove_provider(this);
		OpenGL::remove_active(this);

	}


	void GL3GraphicContextProvider::add_disposable(DisposableObject *disposable)
	{
		disposable_objects.push_back(disposable);
	}

	void GL3GraphicContextProvider::remove_disposable(DisposableObject *disposable)
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

	ProcAddress *GL3GraphicContextProvider::get_proc_address(const std::string& function_name) const
	{
		return render_window->get_proc_address(function_name);
	}

	void GL3GraphicContextProvider::check_opengl_version()
	{
		int version_major = 0;
		int version_minor = 0;

		get_opengl_version(version_major, version_minor);
		if (version_major < 3)
			throw Exception(string_format("This application requires OpenGL 3.0 or above. Your hardware only supports OpenGL %1.%2. Try updating your drivers, or upgrade to a newer graphics card.", version_major, version_minor));
	}

	void GL3GraphicContextProvider::get_opengl_version(int &version_major, int &version_minor) const
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

	void GL3GraphicContextProvider::calculate_shading_language_version()
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

	int GL3GraphicContextProvider::get_max_attributes()
	{
		OpenGL::set_active(this);
		GLint max_attributes = 0;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_attributes);
		if (max_attributes < 16)
			max_attributes = 16;
		return max_attributes;
	}

	Size GL3GraphicContextProvider::get_max_texture_size() const
	{
		OpenGL::set_active(this);
		GLint max_size = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_size);
		return Size(max_size, max_size);
	}

	Size GL3GraphicContextProvider::get_display_window_size() const
	{
		return render_window->get_viewport().get_size();
	}

	float GL3GraphicContextProvider::get_pixel_ratio() const
	{
		return render_window->get_pixel_ratio();
	}

	std::shared_ptr<RenderBuffer> GL3GraphicContextProvider::create_render_buffer(int width, int height, TextureFormat texture_format, int multisample_samples)
	{
		return std::make_shared<GL3RenderBufferProvider>(width, height, texture_format, multisample_samples);
	}

	std::shared_ptr<RasterizerState> GL3GraphicContextProvider::create_rasterizer_state(const RasterizerStateDescription &desc)
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

	std::shared_ptr<BlendState> GL3GraphicContextProvider::create_blend_state(const BlendStateDescription &desc)
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

	std::shared_ptr<DepthStencilState> GL3GraphicContextProvider::create_depth_stencil_state(const DepthStencilStateDescription &desc)
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

	std::shared_ptr<ShaderObject> GL3GraphicContextProvider::create_shader(ShaderType type, const std::string &source)
	{
		return std::make_shared<GL3ShaderObjectProvider>(type, source);
	}

	std::shared_ptr<ShaderObject> GL3GraphicContextProvider::create_shader(ShaderType type, const void *bytecode, int bytecode_size)
	{
		throw Exception("Shader Objects with bytecode are not supported for OpenGL");
	}

	std::shared_ptr<ProgramObject> GL3GraphicContextProvider::create_program()
	{
		return std::make_shared<GL3ProgramObjectProvider>();
	}

	std::shared_ptr<OcclusionQuery> GL3GraphicContextProvider::create_occlusion_query()
	{
		return std::make_shared<GL3OcclusionQueryProvider>(this);
	}

	std::shared_ptr<FrameBuffer> GL3GraphicContextProvider::create_frame_buffer()
	{
		return std::make_shared<GL3FrameBufferProvider>(this);
	}

	std::shared_ptr<StorageBuffer> GL3GraphicContextProvider::create_storage_buffer(int size, int stride, BufferUsage usage)
	{
		return std::make_shared<GL3StorageBufferProvider>(size, stride, usage);
	}

	std::shared_ptr<StorageBuffer> GL3GraphicContextProvider::create_storage_buffer(const void *data, int size, int stride, BufferUsage usage)
	{
		return std::make_shared<GL3StorageBufferProvider>(data, size, stride, usage);
	}

	std::shared_ptr<ElementArrayBuffer> GL3GraphicContextProvider::create_element_array_buffer(int size, BufferUsage usage)
	{
		return std::make_shared<GL3ElementArrayBufferProvider>(size, usage);
	}

	std::shared_ptr<ElementArrayBuffer> GL3GraphicContextProvider::create_element_array_buffer(const void *data, int size, BufferUsage usage)
	{
		return std::make_shared<GL3ElementArrayBufferProvider>(data, size, usage);
	}

	std::shared_ptr<VertexArrayBuffer> GL3GraphicContextProvider::create_vertex_array_buffer(int size, BufferUsage usage)
	{
		return std::make_shared<GL3VertexArrayBufferProvider>(size, usage);
	}

	std::shared_ptr<VertexArrayBuffer> GL3GraphicContextProvider::create_vertex_array_buffer(const void *data, int size, BufferUsage usage)
	{
		return std::make_shared<GL3VertexArrayBufferProvider>(data, size, usage);
	}

	std::shared_ptr<UniformBuffer> GL3GraphicContextProvider::create_uniform_buffer(int size, BufferUsage usage)
	{
		return std::make_shared<GL3UniformBufferProvider>(size, usage);
	}

	std::shared_ptr<UniformBuffer> GL3GraphicContextProvider::create_uniform_buffer(const void *data, int size, BufferUsage usage)
	{
		return std::make_shared<GL3UniformBufferProvider>(data, size, usage);
	}

	std::shared_ptr<TransferBuffer> GL3GraphicContextProvider::create_transfer_buffer(int size, BufferUsage usage)
	{
		return std::make_shared<GL3TransferBufferProvider>(size, usage);
	}

	std::shared_ptr<TransferBuffer> GL3GraphicContextProvider::create_transfer_buffer(const void *data, int size, BufferUsage usage)
	{
		return std::make_shared<GL3TransferBufferProvider>(data, size, usage);
	}

	std::shared_ptr<PrimitivesArray> GL3GraphicContextProvider::create_primitives_array()
	{
		return std::make_shared<GL3PrimitivesArrayProvider>(this);
	}

	std::shared_ptr<Texture1D> GL3GraphicContextProvider::create_texture_1d(int width, TextureFormat texture_format, int levels)
	{
		return std::make_shared<Texture1DImpl<GL3TextureProvider>>(GL3TextureProvider::InitData(), width, texture_format, levels);
	}

	std::shared_ptr<Texture1DArray> GL3GraphicContextProvider::create_texture_1d_array(int width, int array_size, TextureFormat texture_format, int levels)
	{
		return std::make_shared<Texture1DArrayImpl<GL3TextureProvider>>(GL3TextureProvider::InitData(), width, array_size, texture_format, levels);
	}

	std::shared_ptr<Texture2D> GL3GraphicContextProvider::create_texture_2d(int width, int height, TextureFormat texture_format, int levels)
	{
		return std::make_shared<Texture2DImpl<GL3TextureProvider>>(GL3TextureProvider::InitData(), width, height, texture_format, levels);
	}

	std::shared_ptr<Texture2DArray> GL3GraphicContextProvider::create_texture_2d_array(int width, int height, int array_size, TextureFormat texture_format, int levels)
	{
		return std::make_shared<Texture2DArrayImpl<GL3TextureProvider>>(GL3TextureProvider::InitData(), width, height, array_size, texture_format, levels);
	}

	std::shared_ptr<Texture3D> GL3GraphicContextProvider::create_texture_3d(int width, int height, int depth, TextureFormat texture_format, int levels)
	{
		return std::make_shared<Texture3DImpl<GL3TextureProvider>>(GL3TextureProvider::InitData(), width, height, depth, texture_format, levels);
	}

	std::shared_ptr<TextureCube> GL3GraphicContextProvider::create_texture_cube(int width, int height, TextureFormat texture_format, int levels)
	{
		return std::make_shared<TextureCubeImpl<GL3TextureProvider>>(GL3TextureProvider::InitData(), width, height, texture_format, levels);
	}

	std::shared_ptr<TextureCubeArray> GL3GraphicContextProvider::create_texture_cube_array(int width, int height, int array_size, TextureFormat texture_format, int levels)
	{
		return std::make_shared<TextureCubeArrayImpl<GL3TextureProvider>>(GL3TextureProvider::InitData(), width, height, array_size, texture_format, levels);
	}

	std::shared_ptr<TransferTexture> GL3GraphicContextProvider::create_transfer_texture(const void *data, const Size &size, PixelBufferDirection direction, TextureFormat format, BufferUsage usage)
	{
		return std::make_shared<GL3PixelBufferProvider>(data, size, direction, format, usage);
	}

	void GL3GraphicContextProvider::set_rasterizer_state(const RasterizerStatePtr &state)
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
	}

	void GL3GraphicContextProvider::set_blend_state(const BlendStatePtr &state, const Colorf &blend_color, unsigned int sample_mask)
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
	}

	void GL3GraphicContextProvider::set_depth_stencil_state(const DepthStencilStatePtr &state, int stencil_ref)
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
	}

	std::shared_ptr<PixelBuffer> GL3GraphicContextProvider::get_pixeldata(const Rect& rect, TextureFormat texture_format, bool clamp) const
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

		Size display_size = get_display_window_size();

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_PACK_ROW_LENGTH, pbuf->pitch() / pbuf->bytes_per_pixel());
		glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_PACK_SKIP_ROWS, 0);
		glReadPixels(rect.left, display_size.height - rect.bottom, rect.get_width(), rect.get_height(), tf.pixel_format, tf.pixel_datatype, pbuf->data());
		pbuf->flip_vertical();
		return pbuf;
	}

	void GL3GraphicContextProvider::set_uniform_buffer(int index, const UniformBufferPtr &buffer)
	{
		if (buffer)
		{
			OpenGL::set_active(this);
			glBindBufferBase(GL_UNIFORM_BUFFER, index, static_cast<GL3UniformBufferProvider*>(buffer.get())->get_handle());
		}
		else
		{
			OpenGL::set_active(this);
			glBindBufferBase(GL_UNIFORM_BUFFER, index, 0);
		}
	}

	void GL3GraphicContextProvider::set_storage_buffer(int index, const StorageBufferPtr &buffer)
	{
		if (buffer)
		{
			OpenGL::set_active(this);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, static_cast<GL3StorageBufferProvider*>(buffer.get())->get_handle());
		}
		else
		{
			OpenGL::set_active(this);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, 0);
		}
	}

	void GL3GraphicContextProvider::set_texture(int unit_index, const TexturePtr &texture)
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
				GL3TextureProvider *provider = static_cast<GL3TextureProvider*>(texture->texture_object());
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

	void GL3GraphicContextProvider::set_image_texture(int unit_index, const TexturePtr &texture)
	{
		if (texture)
		{
			OpenGL::set_active(this);
			GL3TextureProvider *provider = static_cast<GL3TextureProvider*>(texture->texture_object());
			glBindImageTexture(unit_index, provider->get_handle(), 0, GL_FALSE, 0, GL_READ_WRITE, provider->get_internal_format());
		}
		else
		{
			OpenGL::set_active(this);
			glBindImageTexture(unit_index, 0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
		}
	}

	bool GL3GraphicContextProvider::is_frame_buffer_owner(const FrameBufferPtr &fb)
	{
		GL3FrameBufferProvider *fb_provider = dynamic_cast<GL3FrameBufferProvider *>(fb.get());
		if (fb_provider)
			return fb_provider->get_gc_provider() == this;
		else
			return false;
	}

	void GL3GraphicContextProvider::set_frame_buffer(const FrameBufferPtr &draw_buffer, const FrameBufferPtr &read_buffer)
	{
		_write_frame_buffer = draw_buffer;
		_read_frame_buffer = read_buffer;

		if (draw_buffer && read_buffer)
		{
			GL3FrameBufferProvider *draw_buffer_provider = dynamic_cast<GL3FrameBufferProvider *>(draw_buffer.get());
			GL3FrameBufferProvider *read_buffer_provider = dynamic_cast<GL3FrameBufferProvider *>(read_buffer.get());

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

	void GL3GraphicContextProvider::set_program_object(StandardProgram standard_program)
	{
		set_program_object(standard_programs.get_program_object(standard_program));
	}

	void GL3GraphicContextProvider::set_program_object(const ProgramObjectPtr &program)
	{
		_program_object = program;

		OpenGL::set_active(this);
		if (glUseProgram == nullptr)
			return;

		if (program)
		{
			glUseProgram(static_cast<GL3ProgramObjectProvider*>(program.get())->get_handle());
		}
		else
		{
			glUseProgram(0);
		}
	}

	bool GL3GraphicContextProvider::is_primitives_array_owner(const PrimitivesArrayPtr &prim_array)
	{
		GL3PrimitivesArrayProvider *prim_array_provider = dynamic_cast<GL3PrimitivesArrayProvider *>(prim_array.get());
		if (prim_array_provider)
			return prim_array_provider->get_gc_provider() == this;
		else
			return false;
	}

	void GL3GraphicContextProvider::draw_primitives(PrimitivesType type, int num_vertices, const PrimitivesArrayPtr &primitives_array)
	{
		set_primitives_array(primitives_array);
		draw_primitives_array(type, 0, num_vertices);
		reset_primitives_array();
	}

	void GL3GraphicContextProvider::set_primitives_array(const PrimitivesArrayPtr &primitives_array)
	{
		if (primitives_array)
		{
			GL3PrimitivesArrayProvider *prim_array = static_cast<GL3PrimitivesArrayProvider *>(primitives_array.get());

			OpenGL::set_active(this);
			glBindVertexArray(prim_array->handle);
		}
		else
		{
			OpenGL::set_active(this);
			glBindVertexArray(0);
		}
	}

	void GL3GraphicContextProvider::draw_primitives_array(PrimitivesType type, int offset, int num_vertices)
	{
		OpenGL::set_active(this);
		glDrawArrays(OpenGL::to_enum(type), offset, num_vertices);
	}

	void GL3GraphicContextProvider::draw_primitives_array_instanced(PrimitivesType type, int offset, int num_vertices, int instance_count)
	{
		OpenGL::set_active(this);
		glDrawArraysInstanced(OpenGL::to_enum(type), offset, num_vertices, instance_count);
	}

	void GL3GraphicContextProvider::set_primitives_elements(const ElementArrayBufferPtr &array_provider)
	{
		if (array_provider)
		{
			OpenGL::set_active(this);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<GL3ElementArrayBufferProvider *>(array_provider.get())->get_handle());
		}
		else
		{
			OpenGL::set_active(this);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}

	void GL3GraphicContextProvider::draw_primitives_elements(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset)
	{
		OpenGL::set_active(this);
		glDrawElements(OpenGL::to_enum(type), count, OpenGL::to_enum(indices_type), (const GLvoid*)offset);
	}

	void GL3GraphicContextProvider::draw_primitives_elements_instanced(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset, int instance_count)
	{
		OpenGL::set_active(this);
		glDrawElementsInstanced(OpenGL::to_enum(type), count, OpenGL::to_enum(indices_type), (const GLvoid*)offset, instance_count);
	}

	void GL3GraphicContextProvider::draw_primitives_elements(
		PrimitivesType type,
		int count,
		const ElementArrayBufferPtr &array_provider,
		VertexAttributeDataType indices_type,
		size_t offset)
	{
		OpenGL::set_active(this);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<GL3ElementArrayBufferProvider *>(array_provider.get())->get_handle());
		glDrawElements(OpenGL::to_enum(type), count, OpenGL::to_enum(indices_type), reinterpret_cast<void*>(offset));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void GL3GraphicContextProvider::draw_primitives_elements_instanced(
		PrimitivesType type,
		int count,
		const ElementArrayBufferPtr &array_provider,
		VertexAttributeDataType indices_type,
		size_t offset,
		int instance_count)
	{
		OpenGL::set_active(this);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<GL3ElementArrayBufferProvider *>(array_provider.get())->get_handle());
		glDrawElementsInstanced(OpenGL::to_enum(type), count, OpenGL::to_enum(indices_type), reinterpret_cast<void*>(offset), instance_count);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void GL3GraphicContextProvider::set_scissor(const Rect &rect)
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

	void GL3GraphicContextProvider::reset_scissor()
	{
		OpenGL::set_active(this);
		glDisable(GL_SCISSOR_TEST);
	}

	void GL3GraphicContextProvider::dispatch(int x, int y, int z)
	{
		OpenGL::set_active(this);
		glDispatchCompute(x, y, z);
	}

	void GL3GraphicContextProvider::clear(const Colorf &color)
	{
		OpenGL::set_active(this);
		glClearColor(
			(GLclampf)color.r,
			(GLclampf)color.g,
			(GLclampf)color.b,
			(GLclampf)color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GL3GraphicContextProvider::clear_stencil(int value)
	{
		OpenGL::set_active(this);
		glClearStencil(value);
		glClear(GL_STENCIL_BUFFER_BIT);
	}

	void GL3GraphicContextProvider::clear_depth(float value)
	{
		OpenGL::set_active(this);
		if (glClearDepth)
			glClearDepth(value);
		else
			glClearDepthf(value);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void GL3GraphicContextProvider::on_window_resized()
	{
		window_resized_signal(render_window->get_viewport().get_size());
	}

	void GL3GraphicContextProvider::set_viewport(const Rectf &viewport)
	{
		OpenGL::set_active(this);
		glViewport(
			GLsizei(viewport.left),
			GLsizei(viewport.top),
			GLsizei(viewport.right - viewport.left),
			GLsizei(viewport.bottom - viewport.top));
	}

	void GL3GraphicContextProvider::set_viewport(int index, const Rectf &viewport)
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

	void GL3GraphicContextProvider::set_depth_range(int viewport, float n, float f)
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

	void GL3GraphicContextProvider::set_draw_buffer(DrawBuffer buffer)
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

	void GL3GraphicContextProvider::make_current() const
	{
		render_window->make_current();
	}

	void GL3GraphicContextProvider::flush()
	{
		OpenGL::set_active(this);
		glFlush();
	}

	const DisplayWindowProvider & GL3GraphicContextProvider::get_render_window() const
	{
		return *render_window;
	}
}
