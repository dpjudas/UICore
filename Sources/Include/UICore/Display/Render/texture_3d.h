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

#include "texture.h"

namespace uicore
{
	/// \brief 3D texture object class.
	class Texture3D : public virtual Texture
	{
	public:
		/// \brief Constructs a 3D texture
		///
		/// \param context = Graphic Context
		/// \param width = value
		/// \param height = value
		/// \param depth = value
		/// \param internal_format = Texture Format
		/// \param levels = Mipmap levels for the texture. 0 = all levels
		static std::shared_ptr<Texture3D> create(const GraphicContextPtr &context, int width, int height, int depth, TextureFormat texture_format = tf_rgba8, int levels = 1);
		static std::shared_ptr<Texture3D> create(const GraphicContextPtr &context, const Vec3i &size, TextureFormat texture_format = tf_rgba8, int levels = 1);

		/// \brief Get the texture width.
		virtual int width() const = 0;

		/// \brief Get the texture height.
		virtual int height() const = 0;

		/// \brief Get the texture depth.
		virtual int depth() const = 0;

		/// \brief Get the texture size.
		Vec3i size() const { return Vec3i(width(), height(), depth()); }

		/// \brief Get the texture wrap mode for the s coordinate.
		virtual TextureWrapMode wrap_mode_s() const = 0;

		/// \brief Get the texture wrap mode for the t coordinate.
		virtual TextureWrapMode wrap_mode_t() const = 0;

		/// \brief Get the texture wrap mode for the r coordinate.
		virtual TextureWrapMode wrap_mode_r() const = 0;

		/// \brief Upload image to texture.
		///
		/// \param context Graphic context to use for the request
		/// \param image Image to upload.
		/// \param level Mipmap level-of-detail number.
		virtual void set_image(const GraphicContextPtr &context, const PixelBufferPtr &image, int depth, int level = 0) = 0;

		/// \brief Upload image to sub texture.
		///
		/// \param context Graphic context to use for the request
		/// \param image Image to upload.
		/// \param level Mipmap level-of-detail number.
		virtual void set_subimage(const GraphicContextPtr &context, int x, int y, int z, const PixelBufferPtr &image, const Rect &src_rect, int level = 0) = 0;

		/// \brief Set the texture wrapping mode.
		virtual void set_wrap_mode(TextureWrapMode wrap_s, TextureWrapMode wrap_t, TextureWrapMode wrap_r) = 0;
	};
}
