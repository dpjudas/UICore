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

#include "UICore/Display/Render/texture.h"
#include "UICore/Display/Render/texture_1d.h"
#include "UICore/Display/Render/texture_1d_array.h"
#include "UICore/Display/Render/texture_2d.h"
#include "UICore/Display/Render/texture_2d_array.h"
#include "UICore/Display/Render/texture_3d.h"
#include "UICore/Display/Render/texture_cube.h"
#include "UICore/Display/Render/texture_cube_array.h"
#include "UICore/Display/Image/pixel_buffer.h"
#include <memory>

namespace uicore
{
	class TextureObject
	{
	public:
		virtual ~TextureObject() { }
	};

	template<typename TextureObject>
	class Texture1DImpl : public Texture1D
	{
	public:
		Texture1DImpl(const typename TextureObject::HandleInit &init) : texture(init) { }
		Texture1DImpl(const typename TextureObject::InitData &init, int size, TextureFormat texture_format, int levels) : texture(init, texture_1d, size, 1, 1, 1, texture_format, levels) { }

		TextureObject *texture_object() override { return &texture; }

		float min_lod() const override { return _min_lod; }
		float max_lod() const override { return _max_lod; }
		float lod_bias() const override { return _lod_bias; }
		int base_level() const override { return _base_level; }
		int max_level() const override  { return _max_level; }
		TextureFilter min_filter() const override { return _min_filter; }
		TextureFilter mag_filter() const override { return _mag_filter; }
		TextureCompareMode compare_mode() const override { return _compare_mode; }
		CompareFunction compare_function() const override { return _compare_function; }
		void generate_mipmap() override { texture.generate_mipmap(); }
		void set_min_lod(float min_lod) override { _min_lod = min_lod; texture.set_min_lod(min_lod); }
		void set_max_lod(float max_lod) override { _max_lod = max_lod; texture.set_max_lod(max_lod); }
		void set_lod_bias(float lod_bias) override { _lod_bias = lod_bias; texture.set_lod_bias(_lod_bias); }
		void set_base_level(int base_level) override { _base_level = base_level;  texture.set_base_level(base_level); }
		void set_max_level(int max_level) override { _max_level = max_level; texture.set_max_level(max_level); }
		void set_min_filter(TextureFilter filter) override { _min_filter = filter; texture.set_min_filter(filter); }
		void set_mag_filter(TextureFilter filter) override { _mag_filter = filter; texture.set_mag_filter(filter); }
		void set_max_anisotropy(float max_anisotropy) override { texture.set_max_anisotropy(max_anisotropy); }
		void set_texture_compare(TextureCompareMode mode, CompareFunction func) override { _compare_mode = mode; _compare_function = func; texture.set_texture_compare(mode, func); }

		int size() const override { return _width; }
		TextureWrapMode wrap_mode_s() const override { return _wrap_mode_s; }
		void set_image(GraphicContext &context, const PixelBuffer &image, int level) override { texture.copy_from(context, 0, 0, 0, level, image, image.get_size()); }
		void set_subimage(GraphicContext &context, int x, const PixelBuffer &image, const int src_x, const int src_width, int level) override { texture.copy_from(context, x, 0, 0, level, image, Rect(src_x, 0, src_x + src_width, 1)); }
		void set_wrap_mode(TextureWrapMode wrap_s) override { _wrap_mode_s = wrap_s; texture.set_wrap_mode(wrap_s); }

	private:
		int _width = 0;
		float _min_lod = 0.0f;
		float _max_lod = 0.0f;
		float _lod_bias = 0.0f;
		int _base_level = 0.0f;
		int _max_level = 0.0f;
		TextureFilter _min_filter = filter_linear;
		TextureFilter _mag_filter = filter_linear;
		TextureCompareMode _compare_mode = comparemode_none;
		CompareFunction _compare_function = compare_lequal;
		TextureWrapMode _wrap_mode_s = wrap_clamp_to_edge;
		TextureObject texture;
	};

	template<typename TextureObject>
	class Texture1DArrayImpl : public Texture1DArray
	{
	public:
		Texture1DArrayImpl(const typename TextureObject::HandleInit &init) : texture(init) { }
		Texture1DArrayImpl(const typename TextureObject::InitData &init, int size, int array_size, TextureFormat texture_format, int levels) : texture(init, texture_1d_array, size, 1, 1, array_size, texture_format, levels) { }

		TextureObject *texture_object() override { return &texture; }

		float min_lod() const override { return _min_lod; }
		float max_lod() const override { return _max_lod; }
		float lod_bias() const override { return _lod_bias; }
		int base_level() const override { return _base_level; }
		int max_level() const override  { return _max_level; }
		TextureFilter min_filter() const override { return _min_filter; }
		TextureFilter mag_filter() const override { return _mag_filter; }
		TextureCompareMode compare_mode() const override { return _compare_mode; }
		CompareFunction compare_function() const override { return _compare_function; }
		void generate_mipmap() override { texture.generate_mipmap(); }
		void set_min_lod(float min_lod) override { _min_lod = min_lod; texture.set_min_lod(min_lod); }
		void set_max_lod(float max_lod) override { _max_lod = max_lod; texture.set_max_lod(max_lod); }
		void set_lod_bias(float lod_bias) override { _lod_bias = lod_bias; texture.set_lod_bias(_lod_bias); }
		void set_base_level(int base_level) override { _base_level = base_level;  texture.set_base_level(base_level); }
		void set_max_level(int max_level) override { _max_level = max_level; texture.set_max_level(max_level); }
		void set_min_filter(TextureFilter filter) override { _min_filter = filter; texture.set_min_filter(filter); }
		void set_mag_filter(TextureFilter filter) override { _mag_filter = filter; texture.set_mag_filter(filter); }
		void set_max_anisotropy(float max_anisotropy) override { texture.set_max_anisotropy(max_anisotropy); }
		void set_texture_compare(TextureCompareMode mode, CompareFunction func) override { _compare_mode = mode; _compare_function = func; texture.set_texture_compare(mode, func); }

		int size() const override { return _width; }
		int array_size() const override { return _array_size; }
		TextureWrapMode wrap_mode_s() const override { return _wrap_mode_s; }
		void set_image(GraphicContext &context, int array_index, const PixelBuffer &image, int level) override { texture.copy_from(context, 0, 0, array_index, level, image, image.get_size()); }
		void set_subimage(GraphicContext &context, int array_index, int x, const PixelBuffer &image, const int src_x, const int src_width, int level) override { texture.copy_from(context, x, 0, array_index, level, image, Rect(src_x, 0, src_x + src_width, 1)); }
		void set_wrap_mode(TextureWrapMode wrap_s) override { _wrap_mode_s = wrap_s; texture.set_wrap_mode(wrap_s); }

	private:
		int _width = 0;
		int _array_size = 0;
		float _min_lod = 0.0f;
		float _max_lod = 0.0f;
		float _lod_bias = 0.0f;
		int _base_level = 0.0f;
		int _max_level = 0.0f;
		TextureFilter _min_filter = filter_linear;
		TextureFilter _mag_filter = filter_linear;
		TextureCompareMode _compare_mode = comparemode_none;
		CompareFunction _compare_function = compare_lequal;
		TextureWrapMode _wrap_mode_s = wrap_clamp_to_edge;
		TextureObject texture;
	};

	template<typename TextureObject>
	class Texture2DImpl : public Texture2D
	{
	public:
		Texture2DImpl(const typename TextureObject::HandleInit &init) : texture(init) { }
		Texture2DImpl(const typename TextureObject::InitData &init, int width, int height, TextureFormat texture_format, int levels) : texture(init, texture_2d, width, height, 1, 1, texture_format, levels) { }

		TextureObject *texture_object() override { return &texture; }

		float min_lod() const override { return _min_lod; }
		float max_lod() const override { return _max_lod; }
		float lod_bias() const override { return _lod_bias; }
		int base_level() const override { return _base_level; }
		int max_level() const override  { return _max_level; }
		TextureFilter min_filter() const override { return _min_filter; }
		TextureFilter mag_filter() const override { return _mag_filter; }
		TextureCompareMode compare_mode() const override { return _compare_mode; }
		CompareFunction compare_function() const override { return _compare_function; }
		void generate_mipmap() override { texture.generate_mipmap(); }
		void set_min_lod(float min_lod) override { _min_lod = min_lod; texture.set_min_lod(min_lod); }
		void set_max_lod(float max_lod) override { _max_lod = max_lod; texture.set_max_lod(max_lod); }
		void set_lod_bias(float lod_bias) override { _lod_bias = lod_bias; texture.set_lod_bias(_lod_bias); }
		void set_base_level(int base_level) override { _base_level = base_level;  texture.set_base_level(base_level); }
		void set_max_level(int max_level) override { _max_level = max_level; texture.set_max_level(max_level); }
		void set_min_filter(TextureFilter filter) override { _min_filter = filter; texture.set_min_filter(filter); }
		void set_mag_filter(TextureFilter filter) override { _mag_filter = filter; texture.set_mag_filter(filter); }
		void set_max_anisotropy(float max_anisotropy) override { texture.set_max_anisotropy(max_anisotropy); }
		void set_texture_compare(TextureCompareMode mode, CompareFunction func) override { _compare_mode = mode; _compare_function = func; texture.set_texture_compare(mode, func); }

		int width() const override { return _width; }
		int height() const override { return _height; }
		TextureWrapMode wrap_mode_s() const override { return _wrap_mode_s; }
		TextureWrapMode wrap_mode_t() const override { return _wrap_mode_t; }
		PixelBuffer pixeldata(GraphicContext &gc, int level = 0) const override { return texture.get_pixeldata(gc, tf_rgba8, level); }
		PixelBuffer pixeldata(GraphicContext &gc, TextureFormat texture_format, int level = 0) const override { return texture.get_pixeldata(gc, texture_format, level); }
		void set_image(GraphicContext &context, const PixelBuffer &image, int level) override { texture.copy_from(context, 0, 0, 0, level, image, image.get_size()); }
		void set_subimage(GraphicContext &context, int x, int y, const PixelBuffer &image, const Rect &src_rect, int level) override { texture.copy_from(context, x, y, 0, level, image, src_rect); }
		void copy_image_from(GraphicContext &context, int level, TextureFormat texture_format) override { texture.copy_image_from(0, 0, _width, _height, level, texture_format, context.get_provider()); }
		void copy_image_from(GraphicContext &context, int x, int y, int width, int height, int level, TextureFormat texture_format) override { texture.copy_image_from(x, y, width, height, level, texture_format, context.get_provider()); }
		void copy_subimage_from(GraphicContext &context, int offset_x, int offset_y, int x, int y, int width, int height, int level) override { texture.copy_subimage_from(offset_x, offset_y, x, y, width, height, level, context.get_provider()); }
		void set_wrap_mode(TextureWrapMode wrap_s, TextureWrapMode wrap_t) override { _wrap_mode_s = wrap_s; _wrap_mode_t = wrap_t; texture.set_wrap_mode(wrap_s, wrap_t); }

	private:
		int _width = 0;
		int _height = 0;
		float _min_lod = 0.0f;
		float _max_lod = 0.0f;
		float _lod_bias = 0.0f;
		int _base_level = 0.0f;
		int _max_level = 0.0f;
		TextureFilter _min_filter = filter_linear;
		TextureFilter _mag_filter = filter_linear;
		TextureCompareMode _compare_mode = comparemode_none;
		CompareFunction _compare_function = compare_lequal;
		TextureWrapMode _wrap_mode_s = wrap_clamp_to_edge;
		TextureWrapMode _wrap_mode_t = wrap_clamp_to_edge;
		TextureObject texture;
	};

	template<typename TextureObject>
	class Texture2DArrayImpl : public Texture2DArray
	{
	public:
		Texture2DArrayImpl(const typename TextureObject::HandleInit &init) : texture(init) { }
		Texture2DArrayImpl(const typename TextureObject::InitData &init, int width, int height, int array_size, TextureFormat texture_format, int levels) : texture(init, texture_2d_array, width, height, 1, array_size, texture_format, levels) { }

		TextureObject *texture_object() override { return &texture; }

		float min_lod() const override { return _min_lod; }
		float max_lod() const override { return _max_lod; }
		float lod_bias() const override { return _lod_bias; }
		int base_level() const override { return _base_level; }
		int max_level() const override  { return _max_level; }
		TextureFilter min_filter() const override { return _min_filter; }
		TextureFilter mag_filter() const override { return _mag_filter; }
		TextureCompareMode compare_mode() const override { return _compare_mode; }
		CompareFunction compare_function() const override { return _compare_function; }
		void generate_mipmap() override { texture.generate_mipmap(); }
		void set_min_lod(float min_lod) override { _min_lod = min_lod; texture.set_min_lod(min_lod); }
		void set_max_lod(float max_lod) override { _max_lod = max_lod; texture.set_max_lod(max_lod); }
		void set_lod_bias(float lod_bias) override { _lod_bias = lod_bias; texture.set_lod_bias(_lod_bias); }
		void set_base_level(int base_level) override { _base_level = base_level;  texture.set_base_level(base_level); }
		void set_max_level(int max_level) override { _max_level = max_level; texture.set_max_level(max_level); }
		void set_min_filter(TextureFilter filter) override { _min_filter = filter; texture.set_min_filter(filter); }
		void set_mag_filter(TextureFilter filter) override { _mag_filter = filter; texture.set_mag_filter(filter); }
		void set_max_anisotropy(float max_anisotropy) override { texture.set_max_anisotropy(max_anisotropy); }
		void set_texture_compare(TextureCompareMode mode, CompareFunction func) override { _compare_mode = mode; _compare_function = func; texture.set_texture_compare(mode, func); }

		int width() const override { return _width; }
		int height() const override { return _height; }
		int array_size() const override { return _array_size; }
		TextureWrapMode wrap_mode_s() const override { return _wrap_mode_s; }
		TextureWrapMode wrap_mode_t() const override { return _wrap_mode_t; }
		void set_image(GraphicContext &context, int array_index, const PixelBuffer &image, int level) override { texture.copy_from(context, 0, 0, array_index, level, image, image.get_size()); }
		void set_subimage(GraphicContext &context, int array_index, int x, int y, const PixelBuffer &image, const Rect &src_rect, int level) override { texture.copy_from(context, x, y, array_index, level, image, src_rect); }
		void set_wrap_mode(TextureWrapMode wrap_s, TextureWrapMode wrap_t) override { _wrap_mode_s = wrap_s; _wrap_mode_t = wrap_t; texture.set_wrap_mode(wrap_s, wrap_t); }

		std::shared_ptr<Texture2D> create_2d_view(int array_index, TextureFormat texture_format, int min_level, int num_levels) override { return std::dynamic_pointer_cast<Texture2D>(texture.create_view(texture_2d, texture_format, min_level, num_levels, array_index, 1)); }

	private:
		int _width = 0;
		int _height = 0;
		int _array_size = 0;
		float _min_lod = 0.0f;
		float _max_lod = 0.0f;
		float _lod_bias = 0.0f;
		int _base_level = 0.0f;
		int _max_level = 0.0f;
		TextureFilter _min_filter = filter_linear;
		TextureFilter _mag_filter = filter_linear;
		TextureCompareMode _compare_mode = comparemode_none;
		CompareFunction _compare_function = compare_lequal;
		TextureWrapMode _wrap_mode_s = wrap_clamp_to_edge;
		TextureWrapMode _wrap_mode_t = wrap_clamp_to_edge;
		TextureObject texture;
	};

	template<typename TextureObject>
	class Texture3DImpl : public Texture3D
	{
	public:
		Texture3DImpl(const typename TextureObject::HandleInit &init) : texture(init) { }
		Texture3DImpl(const typename TextureObject::InitData &init, int width, int height, int depth, TextureFormat texture_format, int levels) : texture(init, texture_3d, width, height, depth, 1, texture_format, levels) { }

		TextureObject *texture_object() override { return &texture; }

		float min_lod() const override { return _min_lod; }
		float max_lod() const override { return _max_lod; }
		float lod_bias() const override { return _lod_bias; }
		int base_level() const override { return _base_level; }
		int max_level() const override  { return _max_level; }
		TextureFilter min_filter() const override { return _min_filter; }
		TextureFilter mag_filter() const override { return _mag_filter; }
		TextureCompareMode compare_mode() const override { return _compare_mode; }
		CompareFunction compare_function() const override { return _compare_function; }
		void generate_mipmap() override { texture.generate_mipmap(); }
		void set_min_lod(float min_lod) override { _min_lod = min_lod; texture.set_min_lod(min_lod); }
		void set_max_lod(float max_lod) override { _max_lod = max_lod; texture.set_max_lod(max_lod); }
		void set_lod_bias(float lod_bias) override { _lod_bias = lod_bias; texture.set_lod_bias(_lod_bias); }
		void set_base_level(int base_level) override { _base_level = base_level;  texture.set_base_level(base_level); }
		void set_max_level(int max_level) override { _max_level = max_level; texture.set_max_level(max_level); }
		void set_min_filter(TextureFilter filter) override { _min_filter = filter; texture.set_min_filter(filter); }
		void set_mag_filter(TextureFilter filter) override { _mag_filter = filter; texture.set_mag_filter(filter); }
		void set_max_anisotropy(float max_anisotropy) override { texture.set_max_anisotropy(max_anisotropy); }
		void set_texture_compare(TextureCompareMode mode, CompareFunction func) override { _compare_mode = mode; _compare_function = func; texture.set_texture_compare(mode, func); }

		int width() const override { return _width; }
		int height() const override { return _height; }
		int depth() const override { return _depth; }
		TextureWrapMode wrap_mode_s() const override { return _wrap_mode_s; }
		TextureWrapMode wrap_mode_t() const override { return _wrap_mode_t; }
		TextureWrapMode wrap_mode_r() const override { return _wrap_mode_r; }
		void set_image(GraphicContext &context, const PixelBuffer &image, int depth, int level) override { texture.copy_from(context, 0, 0, depth, level, image, image.get_size()); }
		void set_subimage(GraphicContext &context, int x, int y, int z, const PixelBuffer &image, const Rect &src_rect, int level) override { texture.copy_from(context, x, y, z, level, image, src_rect); }
		void set_wrap_mode(TextureWrapMode wrap_s, TextureWrapMode wrap_t, TextureWrapMode wrap_r) override { _wrap_mode_s = wrap_s; _wrap_mode_t = wrap_t; _wrap_mode_r = wrap_r; texture.set_wrap_mode(wrap_s, wrap_t); texture.set_wrap_mode(wrap_s, wrap_t, wrap_r); }

	private:
		int _width = 0;
		int _height = 0;
		int _depth = 0;
		float _min_lod = 0.0f;
		float _max_lod = 0.0f;
		float _lod_bias = 0.0f;
		int _base_level = 0.0f;
		int _max_level = 0.0f;
		TextureFilter _min_filter = filter_linear;
		TextureFilter _mag_filter = filter_linear;
		TextureCompareMode _compare_mode = comparemode_none;
		CompareFunction _compare_function = compare_lequal;
		TextureWrapMode _wrap_mode_s = wrap_clamp_to_edge;
		TextureWrapMode _wrap_mode_t = wrap_clamp_to_edge;
		TextureWrapMode _wrap_mode_r = wrap_clamp_to_edge;
		TextureObject texture;
	};

	template<typename TextureObject>
	class TextureCubeImpl : public TextureCube
	{
	public:
		TextureCubeImpl(const typename TextureObject::HandleInit &init) : texture(init) { }
		TextureCubeImpl(const typename TextureObject::InitData &init, int width, int height, TextureFormat texture_format, int levels) : texture(init, texture_cube, width, height, 1, 1, texture_format, levels) { }

		TextureObject *texture_object() override { return &texture; }

		float min_lod() const override { return _min_lod; }
		float max_lod() const override { return _max_lod; }
		float lod_bias() const override { return _lod_bias; }
		int base_level() const override { return _base_level; }
		int max_level() const override  { return _max_level; }
		TextureFilter min_filter() const override { return _min_filter; }
		TextureFilter mag_filter() const override { return _mag_filter; }
		TextureCompareMode compare_mode() const override { return _compare_mode; }
		CompareFunction compare_function() const override { return _compare_function; }
		void generate_mipmap() override { texture.generate_mipmap(); }
		void set_min_lod(float min_lod) override { _min_lod = min_lod; texture.set_min_lod(min_lod); }
		void set_max_lod(float max_lod) override { _max_lod = max_lod; texture.set_max_lod(max_lod); }
		void set_lod_bias(float lod_bias) override { _lod_bias = lod_bias; texture.set_lod_bias(_lod_bias); }
		void set_base_level(int base_level) override { _base_level = base_level;  texture.set_base_level(base_level); }
		void set_max_level(int max_level) override { _max_level = max_level; texture.set_max_level(max_level); }
		void set_min_filter(TextureFilter filter) override { _min_filter = filter; texture.set_min_filter(filter); }
		void set_mag_filter(TextureFilter filter) override { _mag_filter = filter; texture.set_mag_filter(filter); }
		void set_max_anisotropy(float max_anisotropy) override { texture.set_max_anisotropy(max_anisotropy); }
		void set_texture_compare(TextureCompareMode mode, CompareFunction func) override { _compare_mode = mode; _compare_function = func; texture.set_texture_compare(mode, func); }

		int width() const override { return _width; }
		int height() const override { return _height; }
		void set_image(GraphicContext &context, TextureCubeDirection cube_direction, const PixelBuffer &image, int level) override { texture.copy_from(context, 0, 0, static_cast<int>(cube_direction), level, image, image.get_size()); }
		void set_subimage(GraphicContext &context, TextureCubeDirection cube_direction, int x, int y, const PixelBuffer &image, const Rect &src_rect, int level) override { texture.copy_from(context, x, y, static_cast<int>(cube_direction), level, image, src_rect); }

	private:
		int _width = 0;
		int _height = 0;
		float _min_lod = 0.0f;
		float _max_lod = 0.0f;
		float _lod_bias = 0.0f;
		int _base_level = 0.0f;
		int _max_level = 0.0f;
		TextureFilter _min_filter = filter_linear;
		TextureFilter _mag_filter = filter_linear;
		TextureCompareMode _compare_mode = comparemode_none;
		CompareFunction _compare_function = compare_lequal;
		TextureObject texture;
	};

	template<typename TextureObject>
	class TextureCubeArrayImpl : public TextureCubeArray
	{
	public:
		TextureCubeArrayImpl(const typename TextureObject::HandleInit &init) : texture(init) { }
		TextureCubeArrayImpl(const typename TextureObject::InitData &init, int width, int height, int array_size, TextureFormat texture_format, int levels) : texture(init, texture_cube_array, width, height, 1, array_size, texture_format, levels) { }

		TextureObject *texture_object() override { return &texture; }

		float min_lod() const override { return _min_lod; }
		float max_lod() const override { return _max_lod; }
		float lod_bias() const override { return _lod_bias; }
		int base_level() const override { return _base_level; }
		int max_level() const override  { return _max_level; }
		TextureFilter min_filter() const override { return _min_filter; }
		TextureFilter mag_filter() const override { return _mag_filter; }
		TextureCompareMode compare_mode() const override { return _compare_mode; }
		CompareFunction compare_function() const override { return _compare_function; }
		void generate_mipmap() override { texture.generate_mipmap(); }
		void set_min_lod(float min_lod) override { _min_lod = min_lod; texture.set_min_lod(min_lod); }
		void set_max_lod(float max_lod) override { _max_lod = max_lod; texture.set_max_lod(max_lod); }
		void set_lod_bias(float lod_bias) override { _lod_bias = lod_bias; texture.set_lod_bias(_lod_bias); }
		void set_base_level(int base_level) override { _base_level = base_level;  texture.set_base_level(base_level); }
		void set_max_level(int max_level) override { _max_level = max_level; texture.set_max_level(max_level); }
		void set_min_filter(TextureFilter filter) override { _min_filter = filter; texture.set_min_filter(filter); }
		void set_mag_filter(TextureFilter filter) override { _mag_filter = filter; texture.set_mag_filter(filter); }
		void set_max_anisotropy(float max_anisotropy) override { texture.set_max_anisotropy(max_anisotropy); }
		void set_texture_compare(TextureCompareMode mode, CompareFunction func) override { _compare_mode = mode; _compare_function = func; texture.set_texture_compare(mode, func); }

		int width() const override { return _width; }
		int height() const override { return _height; }
		int array_size() const override { return _array_size; }
		void set_image(GraphicContext &context, int array_index, TextureCubeDirection cube_direction, const PixelBuffer &image, int level) override { texture.copy_from(context, 0, 0, array_index * 6 + static_cast<int>(cube_direction), level, image, image.get_size()); }
		void set_subimage(GraphicContext &context, int array_index, TextureCubeDirection cube_direction, int x, int y, const PixelBuffer &image, const Rect &src_rect, int level) override { texture.copy_from(context, x, y, array_index * 6 + static_cast<int>(cube_direction), level, image, src_rect); }

	private:
		int _width = 0;
		int _height = 0;
		int _array_size = 0;
		float _min_lod = 0.0f;
		float _max_lod = 0.0f;
		float _lod_bias = 0.0f;
		int _base_level = 0.0f;
		int _max_level = 0.0f;
		TextureFilter _min_filter = filter_linear;
		TextureFilter _mag_filter = filter_linear;
		TextureCompareMode _compare_mode = comparemode_none;
		CompareFunction _compare_function = compare_lequal;
		TextureObject texture;
	};
}
