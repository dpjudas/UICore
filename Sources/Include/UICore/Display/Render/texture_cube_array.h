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

#include "texture_cube.h"

namespace uicore
{
	/// \brief 2D texture cube array object class.
	class TextureCubeArray : public virtual Texture
	{
	public:
		/// \brief Constructs a Texture
		///
		/// If levels is set to zero it will create a texture containing all mipmap levels
		static std::shared_ptr<TextureCubeArray> create(const GraphicContextPtr &context, int width, int height, int array_size, TextureFormat texture_format = tf_rgba8, int levels = 1);
		static std::shared_ptr<TextureCubeArray> create(const GraphicContextPtr &context, const Size &size, int array_size, TextureFormat texture_format = tf_rgba8, int levels = 1);

		/// \brief Get the texture width.
		virtual int width() const = 0;

		/// \brief Get the texture height.
		virtual int height() const = 0;

		/// \brief Get the texture size.
		Size size() const { return Size(width(), height()); }

		/// \brief Get the texture array size
		virtual int array_size() const = 0;

		/// \brief Upload image to texture.
		///
		/// \param context Graphic context to use for the request
		/// \param array_index Index in the array
		/// \param image Image to upload.
		/// \param level Mipmap level-of-detail number.
		virtual void set_image(const GraphicContextPtr &context, int array_index, TextureCubeDirection cube_direction, const PixelBufferPtr &image, int level = 0) = 0;

		/// \brief Upload image to sub texture.
		///
		/// \param context Graphic context to use for the request
		/// \param array_index Index in the array
		/// \param image Image to upload.
		/// \param level Mipmap level-of-detail number.
		virtual void set_subimage(const GraphicContextPtr &context, int array_index, TextureCubeDirection cube_direction, int x, int y, const PixelBufferPtr &image, const Rect &src_rect, int level = 0) = 0;
		virtual void set_subimage(const GraphicContextPtr &context, int array_index, TextureCubeDirection cube_direction, const Point &point, const PixelBufferPtr &image, const Rect &src_rect, int level = 0)
		{
			set_subimage(context, array_index, cube_direction, point.x, point.y, image, src_rect, level);
		}
	};
}
