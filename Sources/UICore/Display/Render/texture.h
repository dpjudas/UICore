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

#pragma once

#include <memory>
#include "graphic_context.h"
#include "UICore/Display/Image/image_import_description.h"
#include "UICore/Display/Image/texture_format.h"

namespace uicore
{
	class Color;
	class Point;
	class PixelBuffer;
	class PixelBufferSet;
	class PixelFormat;
	class TextureProvider;
	class Texture1D;
	class Texture1DArray;
	class Texture2D;
	class Texture2DArray;
	class Texture3D;
	class TextureCube;
	class TextureCubeArray;
	class TextureObject;

	/// \brief Texture coordinate wrapping modes.
	enum TextureWrapMode
	{
		wrap_clamp_to_edge,
		wrap_repeat,
		wrap_mirrored_repeat
	};

	/// \brief Texture filters.
	enum TextureFilter
	{
		filter_nearest,
		filter_linear,
		filter_nearest_mipmap_nearest,
		filter_nearest_mipmap_linear,
		filter_linear_mipmap_nearest,
		filter_linear_mipmap_linear
	};

	/// \brief Texture compare modes.
	enum TextureCompareMode
	{
		comparemode_none,
		comparemode_compare_r_to_texture
	};

	/// \brief Texture object class.
	class Texture
	{
	public:
		/// \brief Constructs a texture as described by a pixelbuffer set
		static std::shared_ptr<Texture> create(const std::shared_ptr<GraphicContext> &gc, std::shared_ptr<PixelBufferSet> pixelbuffer_set);

		/// \brief Get the minimum level of detail.
		virtual float min_lod() const = 0;

		/// \brief Get the maximum level of detail.
		virtual float max_lod() const = 0;

		/// \brief Get the level of detail bias constant.
		virtual float lod_bias() const = 0;

		/// \brief Get the texture base level.
		virtual int base_level() const = 0;

		/// \brief Get the texture max level.
		virtual int max_level() const = 0;

		/// \brief Get the texture minification filter.
		virtual TextureFilter min_filter() const = 0;

		/// \brief Get the texture magnification filter.
		virtual TextureFilter mag_filter() const = 0;

		/// \brief Get the texture compare mode.
		virtual TextureCompareMode compare_mode() const = 0;

		/// \brief Get the texture compare function.
		virtual CompareFunction compare_function() const = 0;

		/// \brief Generate mipmap textures
		virtual void generate_mipmap() = 0;

		/// \brief Set the minimum level of detail texture parameter.
		virtual void set_min_lod(float min_lod) = 0;

		/// \brief Set the maximum level of detail texture parameter.
		virtual void set_max_lod(float max_lod) = 0;

		/// \brief Sets the level of detail bias constant.
		virtual void set_lod_bias(float lod_bias) = 0;

		/// \brief Sets the texture base level texture parameter.
		virtual void set_base_level(int base_level) = 0;

		/// \brief Sets the texture max level texture parameter.
		virtual void set_max_level(int max_level) = 0;

		/// \brief Set the minification filter.
		virtual void set_min_filter(TextureFilter filter) = 0;

		/// \brief Set the magnification filter.
		virtual void set_mag_filter(TextureFilter filter) = 0;

		/// \brief Set the maximum degree of anisotropy.
		virtual void set_max_anisotropy(float max_anisotropy) = 0;

		/// \brief Sets the texture compare mode and compare function texture parameters.
		virtual void set_texture_compare(TextureCompareMode mode, CompareFunction func) = 0;

		/// \internal Returns the internal texture object
		virtual TextureObject *texture_object() = 0;
	};
}
