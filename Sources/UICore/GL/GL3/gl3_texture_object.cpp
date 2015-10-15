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
#include "gl3_texture_object.h"
#include "gl3_graphic_context.h"
#include "UICore/Display/Image/pixel_buffer.h"
#include "UICore/Display/Render/texture.h"
#include "UICore/Display/Render/texture_impl.h"
#include "UICore/GL/opengl_wrap.h"
#include "UICore/GL/opengl.h"
#include "UICore/Core/System/databuffer.h"
#include "gl3_transfer_texture.h"

namespace uicore
{
	GL3TextureObject::GL3TextureObject(const HandleInit &init)
		: handle(init.handle), texture_type(init.texture_type)
	{
		if (init.orig_texture == nullptr)
		{
			TextureStateTracker state_tracker(texture_type, handle);
			glGetIntegerv(GL_TEXTURE_WIDTH, &dimensions.x);
			glGetIntegerv(GL_TEXTURE_HEIGHT, &dimensions.y);
			glGetIntegerv(GL_TEXTURE_DEPTH, &dimensions.z);
		}
		else
		{
			dimensions = init.orig_texture->dimensions;

			create_initial(init.texture_dimensions);

			TextureFormat_GL tf = OpenGL::get_textureformat(init.texture_format);
			if (!tf.valid)
				throw Exception("Texture format not supported by OpenGL");

			if (!glTextureView)
				throw Exception("glTextureView required OpenGL 4.3");

			glTextureView(handle, texture_type, init.orig_texture->handle, tf.internal_format, init.min_level, init.num_levels, init.min_layer, init.num_layers);
		}
	}

	GL3TextureObject::GL3TextureObject(const InitData &init, TextureDimensions texture_dimensions, int new_width, int new_height, int new_depth, int new_array_size, TextureFormat texture_format, int levels)
		: dimensions(new_width, new_height, new_depth, new_array_size), handle(0), texture_type(0)
	{
		create_initial(texture_dimensions);

		TextureFormat_GL tf = OpenGL::get_textureformat(texture_format);
		if (!tf.valid)
			throw Exception("Texture format not supported by OpenGL");

		gl_internal_format = tf.internal_format;

		TextureStateTracker state_tracker(texture_type, handle);

		// To do: move these settings to API
		GLsizei samples = 1;
		GLboolean fixed_sample_locations = GL_FALSE;

		if (levels == 0)
		{
			do
			{
				levels++;
			} while (max(width() >> levels, 1) != 1 || max(height() >> levels, 1) != 1);
		}

		// Emulate glTexStorage behavior so we can support older versions of OpenGL
		for (int level = 0; level < levels; level++)
		{
			int mip_width = max(width() >> level, 1);
			int mip_height = max(height() >> level, 1);

			if (texture_type == GL_TEXTURE_1D)
			{
				glTexImage1D(GL_TEXTURE_1D, level, tf.internal_format, mip_width, 0, tf.pixel_format, tf.pixel_datatype, nullptr);
			}
			else if (texture_type == GL_TEXTURE_1D_ARRAY)
			{
				glTexImage2D(GL_TEXTURE_1D_ARRAY, level, tf.internal_format, mip_width, array_size(), 0, tf.pixel_format, tf.pixel_datatype, nullptr);
			}
			else if (texture_type == GL_TEXTURE_2D)
			{
				if (PixelBuffer::is_compressed(texture_format))
				{
					unsigned data_size = PixelBuffer::data_size(Size(mip_width, mip_height), texture_format);
					glCompressedTexImage2D(GL_TEXTURE_2D, level, tf.internal_format, mip_width, mip_height, 0, data_size, nullptr);
				}
				else
				{
					glTexImage2D(GL_TEXTURE_2D, level, tf.internal_format, mip_width, mip_height, 0, tf.pixel_format, tf.pixel_datatype, nullptr);
				}
			}
			else if (texture_type == GL_TEXTURE_2D_ARRAY)
			{
				glTexImage3D(GL_TEXTURE_2D_ARRAY, level, tf.internal_format, mip_width, mip_height, array_size(), 0, tf.pixel_format, tf.pixel_datatype, nullptr);
			}
			else if (texture_type == GL_TEXTURE_2D_MULTISAMPLE)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, tf.internal_format, mip_width, mip_height, fixed_sample_locations);
			}
			else if (texture_type == GL_TEXTURE_2D_MULTISAMPLE_ARRAY)
			{
				glTexImage3DMultisample(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, samples, tf.internal_format, mip_width, mip_height, array_size(), fixed_sample_locations);
			}
			else if (texture_type == GL_TEXTURE_CUBE_MAP)
			{
				for (int i = 0; i < 6; i++)
					glTexImage2D(OpenGL::to_cube_target(i), level, tf.internal_format, mip_width, mip_height, 0, tf.pixel_format, tf.pixel_datatype, nullptr);
			}
			else if (texture_type == GL_TEXTURE_CUBE_MAP_ARRAY)
			{
				for (int i = 0; i < 6; i++)
					glTexImage3D(OpenGL::to_cube_target(i), level, tf.internal_format, mip_width, mip_height, array_size(), 0, tf.pixel_format, tf.pixel_datatype, nullptr);
			}
			else
			{
				glTexImage3D(GL_TEXTURE_3D, level, tf.internal_format, mip_width, mip_height, depth(), 0, tf.pixel_format, tf.pixel_datatype, nullptr);
			}
		}
	}

	void GL3TextureObject::create_initial(TextureDimensions texture_dimensions)
	{
		switch (texture_dimensions)
		{
		case texture_1d:
			texture_type = GL_TEXTURE_1D;
			break;
		case texture_1d_array:
			texture_type = GL_TEXTURE_1D_ARRAY;
			break;
		case texture_2d:
			texture_type = GL_TEXTURE_2D;
			break;
		case texture_2d_array:
			texture_type = GL_TEXTURE_2D_ARRAY;
			break;
		case texture_3d:
			texture_type = GL_TEXTURE_3D;
			break;
		case texture_cube:
			texture_type = GL_TEXTURE_CUBE_MAP;
			break;
		case texture_cube_array:
			texture_type = GL_TEXTURE_CUBE_MAP_ARRAY;
			break;
		default:
			throw Exception("Unsupported texture type");
		}

		TextureStateTracker state_tracker(texture_type, 0);
		glGenTextures(1, &handle);
		glBindTexture(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		if (texture_type != GL_TEXTURE_1D)
			glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		if (texture_type == GL_TEXTURE_3D)
			glTexParameteri(texture_type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	GL3TextureObject::~GL3TextureObject()
	{
		dispose();
	}

	void GL3TextureObject::on_dispose()
	{
		if (handle)
		{
			if (OpenGL::set_active())
			{
				glDeleteTextures(1, &handle);
				handle = 0;
			}
		}
	}

	void GL3TextureObject::generate_mipmap()
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glGenerateMipmap(texture_type);
	}

	PixelBufferPtr GL3TextureObject::get_pixeldata(const GraphicContextPtr &gc, TextureFormat texture_format, int level) const
	{
		throw_if_disposed();

		TextureStateTracker state_tracker(texture_type, handle);

		TextureFormat_GL tf = OpenGL::get_textureformat(texture_format);
		if (tf.valid)
		{
			auto buffer = PixelBuffer::create(width(), height(), texture_format);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, buffer->pitch() / buffer->bytes_per_pixel());
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
			glGetTexImage(texture_type, level, tf.pixel_format, tf.pixel_datatype, buffer->data());
			return buffer;
		}
		else
		{
			auto buffer = PixelBuffer::create(width(), height(), tf_bgra8);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, buffer->pitch() / buffer->bytes_per_pixel());
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
			glGetTexImage(texture_type, level, GL_RGBA, GL_UNSIGNED_BYTE, buffer->data());
			return buffer->to_format(texture_format);
		}
	}

	void GL3TextureObject::copy_from(const GraphicContextPtr &gc, int x, int y, int slice, int level, const PixelBufferPtr &src, const Rect &src_rect)
	{
		throw_if_disposed();
		if (src_rect.left < 0 || src_rect.top < 0 || src_rect.right > src->width() || src_rect.bottom > src->height())
			throw Exception("Rectangle out of bounds");

		TextureFormat_GL tf = OpenGL::get_textureformat(src->format());
		bool conv_needed = !tf.valid;

		PixelBufferPtr src_converted;
		if (conv_needed)
		{
			src_converted = src->to_format(tf_rgba8); // To do: it should use the internal format here (monkey function missing for this)
		}
		else
		{
			src_converted = src;
			tf = OpenGL::get_textureformat(src_converted->format());
		}

		OpenGL::set_active(gc);
		TextureStateTracker state_tracker(texture_type, handle);

		const unsigned char *data = nullptr;
		GL3TransferTexture *buffer_provider = nullptr;
		GLint last_buffer = 0;
		buffer_provider = dynamic_cast<GL3TransferTexture*>(src_converted.get());
		if (buffer_provider)
		{
			glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &last_buffer);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, buffer_provider->get_handle());
		}
		else
		{
			data = src_converted->data<unsigned char>();
		}

		if (src_converted->is_compressed())
		{
			if (Rect(Point(0, 0), src->size()) != src_rect)
				throw Exception("Entire pixel buffer must be used for compressed texture uploads");

			unsigned data_size = src->data_size();

			if (texture_type == GL_TEXTURE_1D)
			{
				glCompressedTexSubImage1D(texture_type, level, x, src_rect.width(), tf.pixel_datatype, data_size, data);
			}
			else if (texture_type == GL_TEXTURE_1D_ARRAY)
			{
				glCompressedTexSubImage2D(texture_type, level, x, slice, src_rect.width(), 1, tf.pixel_datatype, data_size, data);
			}
			else if (texture_type == GL_TEXTURE_2D || texture_type == GL_TEXTURE_2D_MULTISAMPLE)
			{
				glCompressedTexSubImage2D(texture_type, level, x, y, src_rect.width(), src_rect.height(), tf.pixel_datatype, data_size, data);
			}
			else if (texture_type == GL_TEXTURE_2D_ARRAY || texture_type == GL_TEXTURE_2D_MULTISAMPLE_ARRAY || texture_type == GL_TEXTURE_3D)
			{
				glCompressedTexSubImage3D(texture_type, level, x, y, slice, src_rect.width(), src_rect.height(), 1, tf.pixel_datatype, data_size, data);
			}
			else if (texture_type == GL_TEXTURE_CUBE_MAP)
			{
				glCompressedTexSubImage2D(OpenGL::to_cube_target(slice), level, x, y, src_rect.width(), src_rect.height(), tf.pixel_datatype, data_size, data);
			}
			else if (texture_type == GL_TEXTURE_CUBE_MAP_ARRAY)
			{
				glCompressedTexSubImage3D(OpenGL::to_cube_target(slice % 6), level, x, y, slice / 6, src_rect.width(), src_rect.height(), 1, tf.pixel_datatype, data_size, data);
			}
		}
		else
		{
			const int bytes_per_pixel = src_converted->bytes_per_pixel();

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, src_converted->pitch() / bytes_per_pixel);
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, src_rect.left);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, src_rect.top);

			if (texture_type == GL_TEXTURE_1D)
			{
				glTexSubImage1D(texture_type, level, x, src_rect.width(), tf.pixel_format, tf.pixel_datatype, data);
			}
			else if (texture_type == GL_TEXTURE_1D_ARRAY)
			{
				glTexSubImage2D(texture_type, level, x, slice, src_rect.width(), 1, tf.pixel_format, tf.pixel_datatype, data);
			}
			else if (texture_type == GL_TEXTURE_2D || texture_type == GL_TEXTURE_2D_MULTISAMPLE)
			{
				glTexSubImage2D(texture_type, level, x, y, src_rect.width(), src_rect.height(), tf.pixel_format, tf.pixel_datatype, data);
			}
			else if (texture_type == GL_TEXTURE_2D_ARRAY || texture_type == GL_TEXTURE_2D_MULTISAMPLE_ARRAY || texture_type == GL_TEXTURE_3D)
			{
				glTexSubImage3D(texture_type, level, x, y, slice, src_rect.width(), src_rect.height(), 1, tf.pixel_format, tf.pixel_datatype, data);
			}
			else if (texture_type == GL_TEXTURE_CUBE_MAP)
			{
				glTexSubImage2D(OpenGL::to_cube_target(slice), level, x, y, src_rect.width(), src_rect.height(), tf.pixel_datatype, tf.pixel_datatype, data);
			}
			else if (texture_type == GL_TEXTURE_CUBE_MAP_ARRAY)
			{
				glTexSubImage3D(OpenGL::to_cube_target(slice % 6), level, x, y, slice / 6, src_rect.width(), src_rect.height(), 1, tf.pixel_format, tf.pixel_datatype, data);
			}

			// Restore these unpack values to the default
			glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
			glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
		}

		if (buffer_provider)
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, last_buffer);
	}

	void GL3TextureObject::copy_image_from(
		int x,
		int y,
		int width,
		int height,
		int level,
		TextureFormat texture_format,
		GraphicContextImpl *gc)
	{
		throw_if_disposed();
		OpenGL::set_active(static_cast<GL3GraphicContext*>(gc));
		TextureStateTracker state_tracker(texture_type, handle);

		TextureFormat_GL tf = OpenGL::get_textureformat(texture_format);
		if (!tf.valid)
			throw Exception("Texture format not supported by OpenGL");

		glCopyTexImage2D(
			GL_TEXTURE_2D,
			level,
			tf.internal_format,
			x, y,
			width, height,
			0);
	}

	void GL3TextureObject::copy_subimage_from(
		int offset_x,
		int offset_y,
		int x,
		int y,
		int width,
		int height,
		int level,
		GraphicContextImpl *gc)
	{
		throw_if_disposed();
		OpenGL::set_active(static_cast<GL3GraphicContext*>(gc));
		TextureStateTracker state_tracker(texture_type, handle);

		glCopyTexSubImage2D(
			GL_TEXTURE_2D,
			level,
			offset_x,
			offset_y,
			x, y,
			width, height);
	}

	void GL3TextureObject::set_min_lod(double min_lod)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameterf(texture_type, GL_TEXTURE_MIN_LOD, (GLfloat)min_lod);
	}

	void GL3TextureObject::set_max_lod(double max_lod)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameterf(texture_type, GL_TEXTURE_MAX_LOD, (GLfloat)max_lod);
	}

	void GL3TextureObject::set_lod_bias(double lod_bias)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameterf(texture_type, GL_TEXTURE_LOD_BIAS, (GLfloat)lod_bias);
	}

	void GL3TextureObject::set_base_level(int base_level)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_BASE_LEVEL, base_level);
	}

	void GL3TextureObject::set_max_level(int max_level)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_MAX_LEVEL, max_level);
	}

	void GL3TextureObject::set_wrap_mode(
		TextureWrapMode wrap_s,
		TextureWrapMode wrap_t,
		TextureWrapMode wrap_r)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, OpenGL::to_enum(wrap_s));
		if (texture_type != GL_TEXTURE_1D)
			glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, OpenGL::to_enum(wrap_t));
		if (texture_type != GL_TEXTURE_1D && texture_type != GL_TEXTURE_2D)
			glTexParameteri(texture_type, GL_TEXTURE_WRAP_R, OpenGL::to_enum(wrap_r));
	}

	void GL3TextureObject::set_wrap_mode(
		TextureWrapMode wrap_s,
		TextureWrapMode wrap_t)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, OpenGL::to_enum(wrap_s));
		glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, OpenGL::to_enum(wrap_t));
	}

	void GL3TextureObject::set_wrap_mode(
		TextureWrapMode wrap_s)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, OpenGL::to_enum(wrap_s));
	}

	void GL3TextureObject::set_min_filter(TextureFilter filter)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, OpenGL::to_enum(filter));
	}

	void GL3TextureObject::set_mag_filter(TextureFilter filter)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);

		// Validation (see opengl spec)
		if (!((filter == filter_nearest) || (filter == filter_linear)))
			throw Exception("filter_nearest, filter_linear are only valid options for the mag filter");

		glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, OpenGL::to_enum(filter));
	}

	void GL3TextureObject::set_max_anisotropy(float v)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameterf(texture_type, GL_TEXTURE_MAX_ANISOTROPY_EXT, v);
	}

	void GL3TextureObject::set_texture_compare(TextureCompareMode mode, CompareFunction func)
	{
		throw_if_disposed();
		TextureStateTracker state_tracker(texture_type, handle);
		glTexParameteri(texture_type, GL_TEXTURE_COMPARE_MODE, OpenGL::to_enum(mode));
		glTexParameteri(texture_type, GL_TEXTURE_COMPARE_FUNC, OpenGL::to_enum(func));
	}

	std::shared_ptr<Texture> GL3TextureObject::create_view(TextureDimensions texture_dimensions, TextureFormat texture_format, int min_level, int num_levels, int min_layer, int num_layers)
	{
		switch (texture_dimensions)
		{
		case texture_1d:
			return std::make_shared<Texture1DImpl<GL3TextureObject>>(HandleInit(this, texture_dimensions, texture_format, min_level, num_levels, min_layer, num_layers));
		case texture_1d_array:
			return std::make_shared<Texture1DArrayImpl<GL3TextureObject>>(HandleInit(this, texture_dimensions, texture_format, min_level, num_levels, min_layer, num_layers));
		case texture_2d:
			return std::make_shared<Texture2DImpl<GL3TextureObject>>(HandleInit(this, texture_dimensions, texture_format, min_level, num_levels, min_layer, num_layers));
		case texture_2d_array:
			return std::make_shared<Texture2DArrayImpl<GL3TextureObject>>(HandleInit(this, texture_dimensions, texture_format, min_level, num_levels, min_layer, num_layers));
		case texture_3d:
			return std::make_shared<Texture3DImpl<GL3TextureObject>>(HandleInit(this, texture_dimensions, texture_format, min_level, num_levels, min_layer, num_layers));
		case texture_cube:
			return std::make_shared<TextureCubeImpl<GL3TextureObject>>(HandleInit(this, texture_dimensions, texture_format, min_level, num_levels, min_layer, num_layers));
		case texture_cube_array:
			return std::make_shared<TextureCubeArrayImpl<GL3TextureObject>>(HandleInit(this, texture_dimensions, texture_format, min_level, num_levels, min_layer, num_layers));
		default:
			throw Exception("Unsupported texture type");
		}
	}

	/////////////////////////////////////////////////////////////////////////////

	TextureStateTracker::TextureStateTracker(GLuint target, GLuint handle)
		: target(target), previous_handle(0)
	{
		OpenGL::set_active();

		switch (target)
		{
		case GL_TEXTURE_1D:
			glGetIntegerv(GL_TEXTURE_BINDING_1D, (GLint *)&previous_handle);
			glBindTexture(GL_TEXTURE_1D, handle);
			break;
		case GL_TEXTURE_1D_ARRAY:
			glGetIntegerv(GL_TEXTURE_BINDING_1D_ARRAY, (GLint *)&previous_handle);
			glBindTexture(GL_TEXTURE_1D_ARRAY, handle);
			break;
		case GL_TEXTURE_2D:
			glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *)&previous_handle);
			glBindTexture(GL_TEXTURE_2D, handle);
			break;
		case GL_TEXTURE_2D_ARRAY:
			glGetIntegerv(GL_TEXTURE_BINDING_2D_ARRAY, (GLint *)&previous_handle);
			glBindTexture(GL_TEXTURE_2D_ARRAY, handle);
			break;
		case GL_TEXTURE_2D_MULTISAMPLE:
			glGetIntegerv(GL_TEXTURE_BINDING_2D_MULTISAMPLE, (GLint *)&previous_handle);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, handle);
			break;
		case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
			glGetIntegerv(GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY, (GLint *)&previous_handle);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, handle);
			break;
		case GL_TEXTURE_3D:
			glGetIntegerv(GL_TEXTURE_BINDING_3D, (GLint *)&previous_handle);
			glBindTexture(GL_TEXTURE_3D, handle);
			break;
		case GL_TEXTURE_CUBE_MAP:
			glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, (GLint *)&previous_handle);
			glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
			break;
		case GL_TEXTURE_CUBE_MAP_ARRAY:
			glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP_ARRAY, (GLint *)&previous_handle);
			glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, handle);
			break;
		default:
			throw Exception("Unknown texture target");
		}
	}

	TextureStateTracker::~TextureStateTracker()
	{
		switch (target)
		{
		case GL_TEXTURE_1D:
			glBindTexture(GL_TEXTURE_1D, previous_handle);
			break;
		case GL_TEXTURE_1D_ARRAY:
			glBindTexture(GL_TEXTURE_1D_ARRAY, previous_handle);
			break;
		case GL_TEXTURE_2D:
			glBindTexture(GL_TEXTURE_2D, previous_handle);
			break;
		case GL_TEXTURE_2D_ARRAY:
			glBindTexture(GL_TEXTURE_2D_ARRAY, previous_handle);
			break;
		case GL_TEXTURE_2D_MULTISAMPLE:
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, previous_handle);
			break;
		case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE_ARRAY, previous_handle);
			break;
		case GL_TEXTURE_3D:
			glBindTexture(GL_TEXTURE_3D, previous_handle);
			break;
		case GL_TEXTURE_CUBE_MAP:
			glBindTexture(GL_TEXTURE_CUBE_MAP, previous_handle);
			break;
		case GL_TEXTURE_CUBE_MAP_ARRAY:
			glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, previous_handle);
			break;
		}
	}
}
