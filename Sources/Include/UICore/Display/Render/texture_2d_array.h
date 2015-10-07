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

#include "texture.h"

namespace uicore
{
	class Texture2D;

	/// 2D texture array object class.
	class Texture2DArray : public virtual Texture
	{
	public:
		/** Constructs a new 2D array texture.
		 *  \param context    Graphic context to construct the texture on.
		 *  \param width      Width of the new texture.
		 *  \param height     Height of the new texture.
		 *  \param array_size Number of textures to allocate in this array.
		 *  \param format     Data format of the new texture.
		 *  \param levels     Number of mipmap levels for the new texture. Setting this to 0 enables all levels.
		 */
		static std::shared_ptr<Texture2DArray> create(const GraphicContextPtr &context, int width, int height, int array_size, TextureFormat texture_format = tf_rgba8, int levels = 1);
		static std::shared_ptr<Texture2DArray> create(const GraphicContextPtr &context, const Size &size, int array_size, TextureFormat texture_format = tf_rgba8, int levels = 1);

		/// Retrieves the actual width of the texture in the display.
		virtual int width() const = 0;

		/// Retrieves the actual height of the texture in the display.
		virtual int height() const = 0;

		/// Retrieves the actual size of the texture.
		Size size() const { return Size{ width(), height() }; }

		/// Returns the number of textures in the array.
		virtual int array_size() const = 0;

		/// Get the texture wrap mode for the s coordinate.
		virtual TextureWrapMode wrap_mode_s() const = 0;

		/// Get the texture wrap mode for the t coordinate.
		virtual TextureWrapMode wrap_mode_t() const = 0;

		/** Upload image to this texture array.
		 *  \param context     Graphic context to use for the request.
		 *  \param array_index Index in the array to upload the image into.
		 *  \param image       Image to upload.
		 *  \param level       Mipmap level-of-detail number.
		 */
		virtual void set_image(const GraphicContextPtr &context, int array_index, const PixelBufferPtr &image, int level = 0) = 0;

		/** Upload image to sub-texture.
		 *  \param context     Graphic context to use for the request.
		 *  \param array_index Index in the array to upload the image into.
		 *  \param x           The horizontal point in the selected texture to write the new sub-texture image onto.
		 *  \param y           The vertical point in the selected texture to write the sub-texture image onto.
		 *  \param point       Point in the selected texture to write the new sub-texture image onto.
		 *  \param image       Image to upload.
		 *  \param level       Mipmap level-of-detail number.
		 */
		virtual void set_subimage(const GraphicContextPtr &context, int array_index, int x, int y, const PixelBufferPtr &image, const Rect &src_rect, int level = 0) = 0;
		void set_subimage(const GraphicContextPtr &context, int array_index, const Point &point, const PixelBufferPtr &image, const Rect &src_rect, int level = 0)
		{
			set_subimage(context, array_index, point.x, point.y, image, src_rect, level);
		}

		virtual void set_wrap_mode(TextureWrapMode wrap_s, TextureWrapMode wrap_t) = 0;

		/// Creates a 2D texture view
		virtual std::shared_ptr<Texture2D> create_2d_view(int array_index, TextureFormat texture_format, int min_level, int num_levels) = 0;
	};
}
