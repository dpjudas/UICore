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
**    Claudio Zanella
*/

#include "UICore/precomp.h"
#include "gl1_graphic_context.h"
#include "gl1_texture_object.h"
#include "gl1_program_object.h"
#include "gl1_render_buffer.h"
#include "gl1_primitives_array.h"
#include "gl1_vertex_array_buffer.h"
#include "gl1_uniform_buffer.h"
#include "gl1_transfer_buffer.h"
#include "UICore/Core/System/databuffer.h"
#include "UICore/Core/Math/vec3.h"
#include "UICore/Core/Text/text.h"
#include "UICore/Core/Text/string_format.h"
#include "UICore/Display/Render/texture.h"
#include "UICore/Display/Render/primitives_array.h"
#include "UICore/Display/Image/pixel_buffer.h"
#include "UICore/Display/Render/frame_buffer.h"
#include "UICore/Display/Render/primitives_array.h"
#include "UICore/Display/Render/program_object.h"
#include "UICore/Display/Window/display_window_provider.h"
#include "UICore/GL/gl_share_list.h"
#include "UICore/GL/opengl.h"
#include "UICore/GL/opengl_wrap.h"
#include "UICore/Display/2D/image.h"
#include "gl1_frame_buffer.h"
#include "UICore/Display/2D/render_batch_triangle.h"

#ifdef WIN32
#include "../Platform/WGL/opengl_window_provider_wgl.h"
#elif defined(__APPLE__)
// To do: add Cocoa window provider here
#elif defined(__ANDROID__)
#include "../Platform/Android/opengl_window_provider_android.h"
#else
#include "../Platform/GLX/opengl_window_provider_glx.h"
#endif

namespace uicore
{
	class GL1SelectedTexture
	{
	public:
		GL1TextureObject *texture = nullptr;
	};

	GL1GraphicContext::GL1GraphicContext(OpenGLWindowProvider * render_window)
		: render_window(render_window),
		prim_arrays_set(false), num_set_tex_arrays(0),
		primitives_array_texture_set(false), primitives_array_texindex_set(false), scissor_enabled(false),
		framebuffer_bound(false)
	{
		check_opengl_version();
		max_texture_coords = get_max_texture_coords();
		// Limit the internal texture coords, to avoid situations where the opengl driver says there are unlimited texture coords
		if (max_texture_coords > 32)
			max_texture_coords = 32;
		// Hack, so the sprite render batcher does not exceed the allowed number of textures
		if (max_texture_coords < RenderBatchTriangle::max_textures)
		{
			if (max_texture_coords > 0)
			{
				RenderBatchTriangle::max_textures = max_texture_coords;
			}
			else
			{
				RenderBatchTriangle::max_textures = 1;
			}
		}

		selected_textures.resize(max_texture_coords);

		internal_program = std::make_shared<GL1ProgramObject>(this);

		// Enable point sprites for legacy opengl
		glEnable(GL_POINT_SPRITE);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		GLShareList::context_created(this);

		set_default_state();
	}

	GL1GraphicContext::~GL1GraphicContext()
	{
		dispose();
	}

	void GL1GraphicContext::on_dispose()
	{
		while (!disposable_objects.empty())
			disposable_objects.front()->dispose();

		GLShareList::context_destroyed(this);
		OpenGL::remove_active(this);
	}

	void GL1GraphicContext::add_disposable(DisposableObject *disposable)
	{
		disposable_objects.push_back(disposable);
	}

	void GL1GraphicContext::remove_disposable(DisposableObject *disposable)
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

	void GL1GraphicContext::check_opengl_version()
	{
		int version_major = 0;
		int version_minor = 0;
		int version_release = 0;

		get_opengl_version(version_major, version_minor, version_release);
		bool valid_version = false;
		if (version_major > 1)
			valid_version = true;
		if ((version_major == 1) && (version_minor >= 3))
			valid_version = true;

		if (!valid_version)
			throw Exception(string_format("This application requires OpenGL 1.3. Your hardware only supports OpenGL %1.%2.%3. Try updating your drivers, or upgrade to a newer graphics card.", version_major, version_minor, version_release));
	}

	void GL1GraphicContext::get_opengl_version(int &version_major, int &version_minor, int &version_release) const
	{
		/*	The GL_VERSION string begins with a version number. The version number uses one of these forms:
			major_number.minor_number
			major_number.minor_number.release_number
			Vendor-specific information may follow the version number. Its format depends on the implementation, but a space always separates the version number and the vendor-specific information.
			All strings are null-terminated.
			If an error is generated, glGetString returns zero.
			*/
		OpenGL::set_active(this);
		std::string version = (char*)glGetString(GL_VERSION);
		version_major = 0;
		version_minor = 0;
		version_release = 0;

		std::vector<std::string> split_version = Text::split(version, ".");
		if (split_version.size() > 0)
			version_major = Text::parse_int32(split_version[0]);
		if (split_version.size() > 1)
			version_minor = Text::parse_int32(split_version[1]);
		if (split_version.size() > 2)
			version_release = Text::parse_int32(split_version[2]);
	}

	ProcAddress *GL1GraphicContext::get_proc_address(const std::string& function_name) const
	{
		return render_window->get_proc_address(function_name);
	}

	int GL1GraphicContext::get_max_texture_coords()
	{
		set_active();
		GLint max_textures = 0;
		glGetIntegerv(GL_MAX_TEXTURE_COORDS, &max_textures);

		if (glGetError() == GL_INVALID_ENUM)
			glGetIntegerv(GL_MAX_TEXTURE_UNITS, &max_textures);

		return max_textures;
	}

	int GL1GraphicContext::max_attributes()
	{
		set_active();
		GLint max_attributes = 0;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max_attributes);
		if (max_attributes < 16)
			max_attributes = 16;
		return max_attributes;
	}


	Size GL1GraphicContext::max_texture_size() const
	{
		set_active();
		GLint max_size = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_size);
		return Size(max_size, max_size);
	}

	Size GL1GraphicContext::display_window_size() const
	{
		return render_window->backing_viewport().get_size();
	}

	float GL1GraphicContext::pixel_ratio() const
	{
		return render_window->pixel_ratio();
	}

	void GL1GraphicContext::set_active() const
	{
		if (framebuffer_bound)
		{
			framebuffer_provider->set_active();
		}
		else
		{
			OpenGL::set_active(this);
		}
	}

	std::shared_ptr<RasterizerState> GL1GraphicContext::create_rasterizer_state(const RasterizerStateDescription &desc)
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

	std::shared_ptr<BlendState> GL1GraphicContext::create_blend_state(const BlendStateDescription &desc)
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

	std::shared_ptr<DepthStencilState> GL1GraphicContext::create_depth_stencil_state(const DepthStencilStateDescription &desc)
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

	std::shared_ptr<ProgramObject> GL1GraphicContext::create_program()
	{
		throw Exception("Program Objects are not supported for OpenGL 1.3");
	}

	std::shared_ptr<ShaderObject> GL1GraphicContext::create_shader(ShaderType type, const std::string &source)
	{
		throw Exception("Shader Objects are not supported for OpenGL 1.3");
	}

	std::shared_ptr<ShaderObject> GL1GraphicContext::create_shader(ShaderType type, const void *bytecode, int bytecode_size)
	{
		throw Exception("Shader Objects are not supported for OpenGL 1.3");
	}

	std::shared_ptr<OcclusionQuery> GL1GraphicContext::create_occlusion_query()
	{
		throw Exception("Occlusion Queries are not supported for OpenGL 1.3");
	}

	std::shared_ptr<FrameBuffer> GL1GraphicContext::create_frame_buffer()
	{
		return std::make_shared<GL1FrameBuffer>(this);
	}

	std::shared_ptr<RenderBuffer> GL1GraphicContext::create_render_buffer(int width, int height, TextureFormat texture_format, int multisample_samples)
	{
		return std::make_shared<GL1RenderBuffer>(this, width, height, texture_format, multisample_samples);
	}

	std::shared_ptr<StorageBuffer> GL1GraphicContext::create_storage_buffer(int size, int stride, BufferUsage usage)
	{
		throw Exception("Storage Buffers are not supported for OpenGL 1.3");
	}

	std::shared_ptr<StorageBuffer> GL1GraphicContext::create_storage_buffer(const void *data, int size, int stride, BufferUsage usage)
	{
		throw Exception("Storage Buffers are not supported for OpenGL 1.3");
	}

	std::shared_ptr<ElementArrayBuffer> GL1GraphicContext::create_element_array_buffer(int size, BufferUsage usage)
	{
		throw Exception("Element Array Buffers are not supported for OpenGL 1.3");
	}

	std::shared_ptr<ElementArrayBuffer> GL1GraphicContext::create_element_array_buffer(const void *data, int size, BufferUsage usage)
	{
		throw Exception("Element Array Buffers are not supported for OpenGL 1.3");
	}

	std::shared_ptr<VertexArrayBuffer> GL1GraphicContext::create_vertex_array_buffer(int size, BufferUsage usage)
	{
		return std::make_shared<GL1VertexArrayBuffer>(size, usage);
	}

	std::shared_ptr<VertexArrayBuffer> GL1GraphicContext::create_vertex_array_buffer(const void *data, int size, BufferUsage usage)
	{
		return std::make_shared<GL1VertexArrayBuffer>(data, size, usage);
	}

	std::shared_ptr<UniformBuffer> GL1GraphicContext::create_uniform_buffer(int size, BufferUsage usage)
	{
		return std::make_shared<GL1UniformBuffer>(size, usage);
	}

	std::shared_ptr<UniformBuffer> GL1GraphicContext::create_uniform_buffer(const void *data, int size, BufferUsage usage)
	{
		return std::make_shared<GL1UniformBuffer>(data, size, usage);
	}

	std::shared_ptr<TransferBuffer> GL1GraphicContext::create_transfer_buffer(int size, BufferUsage usage)
	{
		return std::make_shared<GL1TransferBuffer>(size, usage);
	}

	std::shared_ptr<TransferBuffer> GL1GraphicContext::create_transfer_buffer(const void *data, int size, BufferUsage usage)
	{
		return std::make_shared<GL1TransferBuffer>(data, size, usage);
	}

	std::shared_ptr<PrimitivesArray> GL1GraphicContext::create_primitives_array()
	{
		return std::make_shared<GL1PrimitivesArray>();
	}

	std::shared_ptr<Texture1D> GL1GraphicContext::create_texture_1d(int width, TextureFormat texture_format, int levels)
	{
		return std::make_shared<Texture1DImpl<GL1TextureObject>>(GL1TextureObject::InitData(), width, texture_format, levels);
	}

	std::shared_ptr<Texture1DArray> GL1GraphicContext::create_texture_1d_array(int width, int array_size, TextureFormat texture_format, int levels)
	{
		return std::make_shared<Texture1DArrayImpl<GL1TextureObject>>(GL1TextureObject::InitData(), width, array_size, texture_format, levels);
	}

	std::shared_ptr<Texture2D> GL1GraphicContext::create_texture_2d(int width, int height, TextureFormat texture_format, int levels)
	{
		return std::make_shared<Texture2DImpl<GL1TextureObject>>(GL1TextureObject::InitData(), width, height, texture_format, levels);
	}

	std::shared_ptr<Texture2DArray> GL1GraphicContext::create_texture_2d_array(int width, int height, int array_size, TextureFormat texture_format, int levels)
	{
		return std::make_shared<Texture2DArrayImpl<GL1TextureObject>>(GL1TextureObject::InitData(), width, height, array_size, texture_format, levels);
	}

	std::shared_ptr<Texture3D> GL1GraphicContext::create_texture_3d(int width, int height, int depth, TextureFormat texture_format, int levels)
	{
		return std::make_shared<Texture3DImpl<GL1TextureObject>>(GL1TextureObject::InitData(), width, height, depth, texture_format, levels);
	}

	std::shared_ptr<TextureCube> GL1GraphicContext::create_texture_cube(int width, int height, TextureFormat texture_format, int levels)
	{
		return std::make_shared<TextureCubeImpl<GL1TextureObject>>(GL1TextureObject::InitData(), width, height, texture_format, levels);
	}

	std::shared_ptr<TextureCubeArray> GL1GraphicContext::create_texture_cube_array(int width, int height, int array_size, TextureFormat texture_format, int levels)
	{
		return std::make_shared<TextureCubeArrayImpl<GL1TextureObject>>(GL1TextureObject::InitData(), width, height, array_size, texture_format, levels);
	}

	std::shared_ptr<TransferTexture> GL1GraphicContext::create_transfer_texture(const void *data, const Size &new_size, PixelBufferDirection direction, TextureFormat new_format, BufferUsage usage)
	{
		throw Exception("Transfer textures are not supported for OpenGL 1.3");
	}

	void GL1GraphicContext::set_rasterizer_state(const RasterizerStatePtr &state)
	{
		if (state)
		{
			OpenGLRasterizerState *gl1_state = static_cast<OpenGLRasterizerState*>(state.get());
			if (gl1_state)
			{
				selected_state.rasterizer.set(*gl1_state);
				set_active();
				framebuffer_bound ? framebuffer_provider->set_state(selected_state.rasterizer) : selected_state.rasterizer.apply();

				scissor_enabled = gl1_state->desc.get_enable_scissor();
			}
		}
		else
		{
			set_rasterizer_state(default_rasterizer_state());
		}
	}

	void GL1GraphicContext::set_blend_state(const BlendStatePtr &state, const Colorf &blend_color, unsigned int sample_mask)
	{
		if (state)
		{
			OpenGLBlendState *gl1_state = static_cast<OpenGLBlendState*>(state.get());
			if (gl1_state)
			{
				selected_state.blend.set(gl1_state->desc, blend_color);
				set_active();
				framebuffer_bound ? framebuffer_provider->set_state(selected_state.blend) : selected_state.blend.apply();
			}
		}
		else
		{
			set_blend_state(default_blend_state(), blend_color, sample_mask);
		}
	}

	void GL1GraphicContext::set_depth_stencil_state(const DepthStencilStatePtr &state, int stencil_ref)
	{
		if (state)
		{
			OpenGLDepthStencilState *gl1_state = static_cast<OpenGLDepthStencilState*>(state.get());
			if (gl1_state)
			{
				selected_state.depth_stencil.set(gl1_state->desc);
				set_active();
				framebuffer_bound ? framebuffer_provider->set_state(selected_state.depth_stencil) : selected_state.depth_stencil.apply();
			}
		}
		else
		{
			set_depth_stencil_state(default_depth_stencil_state(), stencil_ref);
		}
	}

	PixelBufferPtr GL1GraphicContext::pixeldata(const Rect& rect, TextureFormat texture_format, bool clamp) const
	{
		GLenum format;
		GLenum type;
		bool found = GL1TextureObject::to_opengl_pixelformat(texture_format, format, type);
		if (!found)
			throw Exception("Unsupported pixel format passed to GraphicContext::pixeldata");

		auto pbuf = PixelBuffer::create(rect.get_width(), rect.get_height(), texture_format);
		set_active();
		if (!framebuffer_bound)
			render_window->is_double_buffered() ? glReadBuffer(GL_BACK) : glReadBuffer(GL_FRONT);

		Size display_size = display_window_size();

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
#ifndef __ANDROID__
		glPixelStorei(GL_PACK_ROW_LENGTH, pbuf->pitch() / pbuf->bytes_per_pixel());
		glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_PACK_SKIP_ROWS, 0);
#endif
		glReadPixels(rect.left, display_size.height - rect.bottom, rect.get_width(), rect.get_height(), format, type, pbuf->data());
		pbuf->flip_vertical();
		return pbuf;
	}

	void GL1GraphicContext::set_uniform_buffer(int index, const UniformBufferPtr &buffer)
	{
		//GL1UniformBuffer *provider = static_cast<GL1UniformBuffer*>(buffer.get_provider());
	}

	void GL1GraphicContext::set_storage_buffer(int index, const StorageBufferPtr &buffer)
	{
	}

	void GL1GraphicContext::set_texture(int unit_index, const TexturePtr &texture)
	{
		if (texture)
		{
			set_active();

			if ((unit_index < 0) || (unit_index >= max_texture_coords))
			{
				throw Exception("Invalid texture unit index in GL1 target");
			}
			selected_textures[unit_index].texture = nullptr;

			if (glActiveTexture != nullptr)
			{
				glActiveTexture(GL_TEXTURE0 + unit_index);
			}
			else if (unit_index > 0)
			{
				return;
			}

			if (!texture)
			{
#ifndef __ANDROID__
				glDisable(GL_TEXTURE_1D);
#endif
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_TEXTURE_3D);
#ifndef __ANDROID__
				glDisable(GL_TEXTURE_CUBE_MAP);
#endif
			}
			else
			{
				GL1TextureObject *provider = static_cast<GL1TextureObject*>(texture->texture_object());
				selected_textures[unit_index].texture = provider;

				glEnable(provider->get_texture_type());
				glBindTexture(provider->get_texture_type(), provider->get_handle());
			}
		}
		else
		{
			set_active();

			if ((unit_index >= 0) && (unit_index < max_texture_coords))
			{
				selected_textures[unit_index].texture = nullptr;
			}

			if (glActiveTexture != nullptr)
			{
				glActiveTexture(GL_TEXTURE0 + unit_index);
			}
			else if (unit_index > 0)
			{
				return;
			}

#ifndef __ANDROID__
			glDisable(GL_TEXTURE_1D);
#endif
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_TEXTURE_3D);
#ifndef __ANDROID__
			glDisable(GL_TEXTURE_CUBE_MAP);
#endif
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void GL1GraphicContext::set_image_texture(int unit_index, const TexturePtr &texture)
	{
	}

	bool GL1GraphicContext::is_frame_buffer_owner(const FrameBufferPtr &fb)
	{
		return true;
	}

	void GL1GraphicContext::set_frame_buffer(const FrameBufferPtr &w_buffer, const FrameBufferPtr &r_buffer)
	{
		_write_frame_buffer = w_buffer;
		_read_frame_buffer = r_buffer;

		if (w_buffer && r_buffer)
		{
			framebuffer_provider = dynamic_cast<GL1FrameBuffer *>(w_buffer.get());
			framebuffer_provider->set_active();
			framebuffer_provider->set_state(selected_state);
			framebuffer_provider->start();

			framebuffer_bound = true;
		}
		else
		{
			if (framebuffer_bound)
			{
				framebuffer_bound = false;

				framebuffer_provider->set_active();
				framebuffer_provider->stop();
				OpenGL::set_active(this);
				selected_state.apply();
			}
		}
	}

	void GL1GraphicContext::set_program_object(StandardProgram standard_program)
	{
	}

	ProgramObjectPtr GL1GraphicContext::program_object() const
	{
		return internal_program;
	}

	void GL1GraphicContext::set_program_object(const ProgramObjectPtr &program)
	{
		throw Exception("GLSL program objects are not supported on OpenGL 1.3");
	}

	bool GL1GraphicContext::is_primitives_array_owner(const PrimitivesArrayPtr &primitives_array)
	{
		return true;
	}

	void GL1GraphicContext::draw_primitives(PrimitivesType type, int num_vertices, const PrimitivesArrayPtr &primitives_array)
	{
		set_primitives_array(primitives_array);
		draw_primitives_array(type, 0, num_vertices);
		reset_primitives_array();
	}

	void GL1GraphicContext::set_primitives_array(const PrimitivesArrayPtr &primitives_array)
	{
		if (primitives_array)
		{
			GL1PrimitivesArray * prim_array = static_cast<GL1PrimitivesArray *>(primitives_array.get());
			if (prim_arrays_set)
				reset_primitives_array();
			set_active();
			prim_arrays_set = true;

			num_set_tex_arrays = 0;

			for (size_t attribute_index = 0; attribute_index < prim_array->attributes.size(); attribute_index++)
			{
				if (!prim_array->attribute_set[attribute_index])
					continue;

				const PrimitivesArrayProvider::VertexData &attribute = prim_array->attributes[attribute_index];

				GL1VertexArrayBuffer *vertex_array_ptr = static_cast<GL1VertexArrayBuffer *>(attribute.array_provider);
				if (!vertex_array_ptr)
					throw Exception("Invalid BindBuffer Provider");

				const char *data_ptr = ((const char *)vertex_array_ptr->get_data()) + attribute.offset;

				switch (attribute_index)
				{
				case 0: // POSITION
					glEnableClientState(GL_VERTEX_ARRAY);
					glVertexPointer(attribute.size, OpenGL::to_enum(attribute.type), attribute.stride, data_ptr);
					break;
				case 1: // COLOR
					glEnableClientState(GL_COLOR_ARRAY);
					glColorPointer(attribute.size, OpenGL::to_enum(attribute.type), attribute.stride, data_ptr);

					break;
				case 2: // TEXTURE
					primitives_array_texture = attribute;
					primitives_array_texture_set = true;
					break;
				case 3: // TEXINDEX
					primitives_array_texindex = attribute;
					primitives_array_texindex_set = true;
					break;
				case 4: // NORMAL
					glEnableClientState(GL_NORMAL_ARRAY);
					glNormalPointer(OpenGL::to_enum(attribute.type), attribute.stride, data_ptr);
					break;
				}
			}
		}
		else
		{
			set_active();

			primitives_array_texture_set = false;
			primitives_array_texindex_set = false;

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
#ifndef __ANDROID__
			glDisableClientState(GL_EDGE_FLAG_ARRAY);
#endif
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

			if (glClientActiveTexture)
			{
				for (int i = 0; i < num_set_tex_arrays; ++i)
				{
					glClientActiveTexture(GL_TEXTURE0 + i);
					glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				}
				num_set_tex_arrays = 0;
			}
			else
			{
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}

			prim_arrays_set = false;
		}
	}

	void GL1GraphicContext::draw_primitives_array(PrimitivesType type, int offset, int num_vertices)
	{
		set_active();

		// Simple condition - No textures set
		if (!primitives_array_texture_set)
		{
			glDrawArrays(OpenGL::to_enum(type), offset, num_vertices);
			return;
		}

		GLenum primitive_type = OpenGL::to_enum(type);

		int total_vertices = offset + num_vertices;

		// Normal condition - No texture index set
		if (!primitives_array_texindex_set)
		{
			set_primitive_texture(0, primitives_array_texture, offset, num_vertices, total_vertices);
			glDrawArrays(primitive_type, offset, num_vertices);
			reset_primitive_texture(0);
			return;
		}

		// For code simplicity, disable all textures
		reset_primitive_texture_all();

		GL1VertexArrayBuffer *vertex_array_ptr = static_cast<GL1VertexArrayBuffer *>(primitives_array_texindex.array_provider);
		if (!vertex_array_ptr)
			throw Exception("Invalid BindBuffer Provider");

		const char *data_ptr = ((const char *)vertex_array_ptr->get_data()) + primitives_array_texindex.offset;

		// Difficult condition - Draw all texture indexes
		while (num_vertices > 0)
		{
			// First, find these values...
			int texture_index;
			int num_vertices_in_group;

			if (primitives_array_texindex.size <= 0)
			{
				// Invalid texindex array (ignore)
				texture_index = 0;
				num_vertices_in_group = num_vertices;
			}
			// Multiple textures possible
			switch (primitives_array_texindex.type)
			{
			case type_int:
			{
				int stride_float;
				if (primitives_array_texindex.stride)
				{
					stride_float = (primitives_array_texindex.stride / sizeof(int));
				}
				else
				{
					stride_float = primitives_array_texindex.size;
				}

				int *dptr = (int *)data_ptr;
				dptr += offset * stride_float;
				num_vertices_in_group = 1;
				texture_index = *dptr;
				dptr += stride_float;
				for (int texcnt = 1; texcnt < num_vertices; ++texcnt)
				{
					// Note, we assume all textures in "primitives_array_texindex.size" are identical
					if ((*dptr) != texture_index)
						break;
					dptr += stride_float;
					num_vertices_in_group++;
				}

				break;
			}
			default:
				throw Exception("Implement me!");
			}

			set_primitive_texture(texture_index, primitives_array_texture, offset, num_vertices_in_group, total_vertices);
			glDrawArrays(primitive_type, offset, num_vertices_in_group);
			reset_primitive_texture(texture_index);

			offset += num_vertices_in_group;
			num_vertices -= num_vertices_in_group;
		}
	}

	void GL1GraphicContext::draw_primitives_array_instanced(PrimitivesType type, int offset, int num_vertices, int instance_count)
	{
		throw Exception("Cannot draw instanced for the OpenGL 1.3 target");
	}

	void GL1GraphicContext::set_primitives_elements(const ElementArrayBufferPtr &array_provider)
	{
		throw Exception("Cannot draw Element Array Buffers for the OpenGL 1.3 target");
	}

	void GL1GraphicContext::draw_primitives_elements(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset)
	{
		throw Exception("Cannot draw Element Array Buffers for the OpenGL 1.3 target");
	}

	void GL1GraphicContext::draw_primitives_elements_instanced(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset, int instance_count)
	{
		throw Exception("Cannot draw instanced for the OpenGL 1.3 target");
	}

	void GL1GraphicContext::draw_primitives_elements(
		PrimitivesType type,
		int count,
		const ElementArrayBufferPtr &array_provider,
		VertexAttributeDataType indices_type,
		size_t offset)
	{
		throw Exception("Cannot draw Element Array Buffers for the OpenGL 1.3 target");
	}

	void GL1GraphicContext::draw_primitives_elements_instanced(
		PrimitivesType type,
		int count,
		const ElementArrayBufferPtr &array_provider,
		VertexAttributeDataType indices_type,
		size_t offset,
		int instance_count)
	{
		throw Exception("Cannot draw Element Array Buffers for the OpenGL 1.3 target");
	}

	void GL1GraphicContext::set_scissor(const Rect &rect)
	{
		set_active();

		if (!scissor_enabled)
			throw Exception("RasterizerState must be set with enable_scissor() for clipping to work");

		glEnable(GL_SCISSOR_TEST);
		glScissor(
			rect.left,
			rect.top,
			rect.get_width(),
			rect.get_height());
	}

	void GL1GraphicContext::reset_scissor()
	{
		set_active();
		glDisable(GL_SCISSOR_TEST);
	}

	void GL1GraphicContext::dispatch(int x, int y, int z)
	{
		throw Exception("Dispatch not supported by OpenGL 1!");
	}

	void GL1GraphicContext::clear(const Colorf &color)
	{
		set_active();
		glClearColor(
			(GLclampf)color.r,
			(GLclampf)color.g,
			(GLclampf)color.b,
			(GLclampf)color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void GL1GraphicContext::clear_stencil(int value)
	{
		set_active();
		glClearStencil(value);
		glClear(GL_STENCIL_BUFFER_BIT);
	}

	void GL1GraphicContext::clear_depth(float value)
	{
		set_active();
		glClearDepth(value);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void GL1GraphicContext::on_window_resized()
	{
		window_resized_signal(render_window->backing_viewport().get_size());
	}

	void GL1GraphicContext::set_viewport(const Rectf &viewport)
	{
		set_active();
		glViewport(
			GLsizei(viewport.left),
			GLsizei(viewport.top),
			GLsizei(viewport.right - viewport.left),
			GLsizei(viewport.bottom - viewport.top));
	}

	void GL1GraphicContext::set_viewport(int index, const Rectf &viewport)
	{
		if (index == 0)
			set_viewport(viewport);
	}

	void GL1GraphicContext::set_depth_range(int viewport, float n, float f)
	{
		if (viewport <= 0)
		{
			set_active();
			glDepthRange(n, f);
		}
	}

	void GL1GraphicContext::set_draw_buffer(DrawBuffer buffer)
	{
		set_active();

		if (!render_window->is_double_buffered())	// Silently fix incorrect render buffers
		{
			if (buffer == buffer_back)
				buffer = buffer_front;
		}

		glDrawBuffer(OpenGL::to_enum(buffer));

	}

	void GL1GraphicContext::make_current() const
	{
		render_window->make_current();
	}

	void GL1GraphicContext::flush()
	{
		set_active();
		glFlush();
	}

	const DisplayWindowProvider & GL1GraphicContext::get_render_window() const
	{
		return *render_window;
	}

	OpenGLWindowProvider & GL1GraphicContext::get_opengl_window()
	{
		return *render_window;
	}

	void GL1GraphicContext::set_primitive_texture(int texture_index, PrimitivesArrayProvider::VertexData &array_texture, int offset, int num_vertices, int total_vertices)
	{
		GL1TextureObject *texture;
		if ((texture_index < 0) || (texture_index >= max_texture_coords))
		{
			texture = nullptr;		// Ignore invalid texture index's
		}
		else
		{
			texture = selected_textures[texture_index].texture;
		}

		if (texture)
		{
			if (glActiveTexture != nullptr)
				glActiveTexture(GL_TEXTURE0 + texture_index);

			glEnable(texture->get_texture_type());
			glBindTexture(texture->get_texture_type(), texture->get_handle());

			if (glClientActiveTexture)
				glClientActiveTexture(GL_TEXTURE0 + texture_index);

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			if (texture->is_power_of_two_texture() || (num_vertices == 0))
			{
				GL1VertexArrayBuffer *vertex_array_ptr = static_cast<GL1VertexArrayBuffer *>(array_texture.array_provider);
				if (!vertex_array_ptr)
					throw Exception("Invalid BindBuffer Provider");

				const char *data_ptr = ((const char *)vertex_array_ptr->get_data()) + array_texture.offset;

				glTexCoordPointer(array_texture.size, OpenGL::to_enum(array_texture.type), array_texture.stride, data_ptr);
			}
			else
			{
				// A hack to handle non-power-of-two textures
				texture->transform_coordinate(array_texture, transformed_coords, offset, num_vertices, total_vertices);
				glTexCoordPointer(array_texture.size, GL_FLOAT, 0, &transformed_coords[0]);
			}
		}
		else
		{
			//Disabled this exception because the sprite render batcher does this intentionally
			//throw Exception("Attempt to draw a texture that was not selected in the GL1 target");
		}

	}

	void GL1GraphicContext::reset_primitive_texture(int texture_index)
	{
		if ((texture_index < 0) || (texture_index >= max_texture_coords))
		{
			return;
		}

		GL1TextureObject *texture = selected_textures[texture_index].texture;
		if (texture)
		{
			if (glActiveTexture != nullptr)
				glActiveTexture(GL_TEXTURE0 + texture_index);

			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
		}

	}

	void GL1GraphicContext::reset_primitive_texture_all()
	{
		for (int cnt = 0; cnt < max_texture_coords; cnt++)
		{
			GL1TextureObject *texture = selected_textures[cnt].texture;
			if (texture)
			{
				if (glActiveTexture != nullptr)
					glActiveTexture(GL_TEXTURE0 + cnt);

				glBindTexture(GL_TEXTURE_2D, 0);
				glDisable(GL_TEXTURE_2D);
			}
		}
	}
}
