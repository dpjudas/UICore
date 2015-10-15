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
	/// \brief 1D texture object class.
	class Texture1D : public virtual Texture
	{
	public:
		/// \brief Constructs a 1D texture
		///
		/// If levels is set to zero it will create a texture containing all mipmap levels
		static std::shared_ptr<Texture1D> create(const GraphicContextPtr &context, int size, TextureFormat texture_format = tf_rgba8, int levels = 1);
		static std::shared_ptr<Texture1D> create(const GraphicContextPtr &context, const PixelBufferPtr &image, bool is_srgb = false);
		static std::shared_ptr<Texture1D> create(const GraphicContextPtr &context, const PixelBufferPtr &image, int src_x, int width, bool is_srgb = false);

		/// \brief Get the texture width.
		virtual int size() const = 0;

		/// \brief Get the texture wrap mode for the s coordinate.
		virtual TextureWrapMode wrap_mode_s() const = 0;

		/// \brief Upload image to texture.
		///
		/// \param context Graphic context to use for the request
		/// \param image Image to upload.
		/// \param level Mipmap level-of-detail number.
		virtual void set_image(const GraphicContextPtr &context, const PixelBufferPtr &image, int level = 0) = 0;

		/// \brief Upload image to sub texture.
		///
		/// \param context Graphic context to use for the request
		/// \param image Image to upload.
		/// \param level Mipmap level-of-detail number.
		virtual void set_subimage(const GraphicContextPtr &context, int x, const PixelBufferPtr &image, const int src_x, const int src_width, int level = 0) = 0;

		virtual void set_wrap_mode(TextureWrapMode wrap_s) = 0;
	};
}
