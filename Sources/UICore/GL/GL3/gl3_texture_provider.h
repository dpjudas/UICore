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
*/

#pragma once

#include "UICore/Display/Image/pixel_buffer.h"
#include "UICore/Display/Render/texture.h"
#include "UICore/Display/Render/texture_1d.h"
#include "UICore/Display/Render/texture_1d_array.h"
#include "UICore/Display/Render/texture_2d.h"
#include "UICore/Display/Render/texture_2d_array.h"
#include "UICore/Display/Render/texture_3d.h"
#include "UICore/Display/Render/texture_cube.h"
#include "UICore/Display/Render/texture_cube_array.h"
#include "UICore/Display/TargetProviders/texture_provider.h"
#include "UICore/Core/System/disposable_object.h"
#include "UICore/GL/opengl.h"
#include "UICore/GL/gl_share_list.h"

namespace uicore
{
	class GL3GraphicContextProvider;

	class GL3TextureProvider : public TextureObject, public GLSharedResource
	{
	public:
		struct HandleInit
		{
			HandleInit(GLuint texture_type, GLuint handle) : texture_type(texture_type), handle(handle) { }
			HandleInit(GL3TextureProvider *orig_texture, TextureDimensions texture_dimensions, TextureFormat texture_format, int min_level, int num_levels, int min_layer, int num_layers)
				: orig_texture(orig_texture), texture_dimensions(texture_dimensions), texture_format(texture_format), min_level(min_level), num_levels(num_levels), min_layer(min_layer), num_layers(num_layers)
			{
			}

			// External texture handle initialization
			GLuint texture_type = 0;
			GLuint handle = 0;

			// Texture view initialization
			GL3TextureProvider *orig_texture = nullptr;
			TextureDimensions texture_dimensions = texture_1d;
			TextureFormat texture_format = tf_rgba8;
			int min_level = 0;
			int num_levels = 0;
			int min_layer = 0;
			int num_layers = 0;
		};

		struct InitData
		{
		};

		GL3TextureProvider(const InitData &init, TextureDimensions texture_dimensions, int width, int height, int depth, int array_size, TextureFormat texture_format, int levels);
		GL3TextureProvider(const HandleInit &init);
		~GL3TextureProvider();

		GLuint get_handle() const { return handle; }
		GLuint get_texture_type() const { return texture_type; }
		GLint get_internal_format() const { return gl_internal_format; }

		void generate_mipmap();

		PixelBufferPtr get_pixeldata(const GraphicContextPtr &gc, TextureFormat texture_format, int level) const;

		void copy_from(const GraphicContextPtr &gc, int x, int y, int slice, int level, const PixelBufferPtr &src, const Rect &src_rect);

		void copy_image_from(int x, int y, int width, int height, int level, TextureFormat texture_format, GraphicContextProvider *gc);
		void copy_subimage_from(int offset_x, int offset_y, int x, int y, int width, int height, int level, GraphicContextProvider *gc);

		void set_min_lod(double min_lod);
		void set_max_lod(double max_lod);
		void set_lod_bias(double lod_bias);
		void set_base_level(int base_level);
		void set_max_level(int max_level);

		void set_wrap_mode(TextureWrapMode wrap_s, TextureWrapMode wrap_t, TextureWrapMode wrap_r);
		void set_wrap_mode(TextureWrapMode wrap_s, TextureWrapMode wrap_t);
		void set_wrap_mode(TextureWrapMode wrap_s);

		void set_min_filter(TextureFilter filter);
		void set_mag_filter(TextureFilter filter);
		void set_max_anisotropy(float v);
		void set_texture_compare(TextureCompareMode mode, CompareFunction func);

		int width() const { return dimensions.x; }
		int height() const { return dimensions.y; }
		int depth() const { return dimensions.z; }
		int array_size() const { return dimensions.w; }

		std::shared_ptr<Texture> create_view(TextureDimensions texture_dimensions, TextureFormat texture_format, int min_level, int num_levels, int min_layer, int num_layers);

	private:
		void create_initial(TextureDimensions texture_dimensions);

		void on_dispose() override;

		Vec4i dimensions;

		GLuint handle = 0;
		GLuint texture_type = 0;
		GLint gl_internal_format = 0;
	};

	class TextureStateTracker
	{
	public:
		TextureStateTracker(GLuint target, GLuint handle);
		~TextureStateTracker();

	private:
		GLuint target;
		GLuint previous_handle;
	};
}
