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
	/// \brief Texture cube directions
	enum TextureCubeDirection
	{
		cl_cube_positive_x,
		cl_cube_negative_x,
		cl_cube_positive_y,
		cl_cube_negative_y,
		cl_cube_positive_z,
		cl_cube_negative_z
	};

	/// \brief 2D texture cube object class.
	class TextureCube : public virtual Texture
	{
	public:
		/// \brief Constructs a cube texture
		///
		/// If levels is set to zero it will create a texture containing all mipmap levels
		static std::shared_ptr<TextureCube> create(const std::shared_ptr<GraphicContext> &context, int width, int height, TextureFormat texture_format = tf_rgba8, int levels = 1);
		static std::shared_ptr<TextureCube> create(const std::shared_ptr<GraphicContext> &context, const Size &size, TextureFormat texture_format = tf_rgba8, int levels = 1);

		/// \brief Get the texture width.
		virtual int width() const = 0;

		/// \brief Get the texture height.
		virtual int height() const = 0;

		/// \brief Get the texture size.
		Size size() const { return Size(width(), height()); }

		/// \brief Upload image to texture.
		///
		/// \param context Graphic context to use for the request
		/// \param image Image to upload.
		/// \param level Mipmap level-of-detail number.
		virtual void set_image(const std::shared_ptr<GraphicContext> &context, TextureCubeDirection cube_direction, const std::shared_ptr<PixelBuffer> &image, int level = 0) = 0;

		/// \brief Upload image to sub texture.
		///
		/// \param context Graphic context to use for the request
		/// \param image Image to upload.
		/// \param level Mipmap level-of-detail number.
		virtual void set_subimage(const std::shared_ptr<GraphicContext> &context, TextureCubeDirection cube_direction, int x, int y, const std::shared_ptr<PixelBuffer> &image, const Rect &src_rect, int level = 0) = 0;
		void set_subimage(const std::shared_ptr<GraphicContext> &context, TextureCubeDirection cube_direction, const Point &point, const std::shared_ptr<PixelBuffer> &image, const Rect &src_rect, int level = 0)
		{
			set_subimage(context, cube_direction, point.x, point.y, image, src_rect, level);
		}
	};
}
