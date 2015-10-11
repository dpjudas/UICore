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
	class IODevice;

	/// 2D texture object class.
	class Texture2D : public virtual Texture
	{
	public:
		/** Constructs a new 2D texture.
		 *  \param context  Graphic context to construct the texture on.
		 *  \param width    Width of the new texture.
		 *  \param height   Height of the new texture.
		 *  \param format   Data format of the new texture.
		 *  \param levels   Number of mipmap levels for the new texture. Setting
		 *                  this to 0 enables all levels.
		 */
		static std::shared_ptr<Texture2D> create(const GraphicContextPtr &context, int width, int height, TextureFormat format = tf_rgba8, int levels = 1);
		static std::shared_ptr<Texture2D> create(const GraphicContextPtr &context, const Size &size, TextureFormat texture_format = tf_rgba8, int levels = 1);

		static std::shared_ptr<Texture2D> create(const GraphicContextPtr &context, const std::string &filename, const ImageImportDescription &import_desc = {});
		static std::shared_ptr<Texture2D> create(const GraphicContextPtr &context, const IODevicePtr &file, const std::string &image_type, const ImageImportDescription &import_desc = {});

		static std::shared_ptr<Texture2D> create(const GraphicContextPtr &context, const PixelBufferPtr &image, bool is_srgb = false);
		static std::shared_ptr<Texture2D> create(const GraphicContextPtr &context, const PixelBufferPtr &image, const Rect &src_rect, bool is_srgb = false);

		/// Retrieves the actual width of the texture in the display.
		virtual int width() const = 0;

		/// Retrieves the actual height of the texture in the display.
		virtual int height() const = 0;

		/// Retrieves the actual size of the texture.
		Size size() const { return Size{ width(), height() }; }

		/** Retrieve image data from this texture.
		 *  \param format Output data format.
		 *  \param level  Mipmap level of the texture to retrieve data from.
		 */
		virtual PixelBufferPtr pixeldata(const GraphicContextPtr &gc, int level = 0) const = 0;
		virtual PixelBufferPtr pixeldata(const GraphicContextPtr &gc, TextureFormat texture_format, int level = 0) const = 0;

		/// Get the texture wrap mode for the s coordinate.
		virtual TextureWrapMode wrap_mode_s() const = 0;

		/// Get the texture wrap mode for the t coordinate.
		virtual TextureWrapMode wrap_mode_t() const = 0;

		/** Upload image to this texture.
		 *  \param context Graphic context to use for the request.
		 *  \param image   Image to upload.
		 *  \param level   Mipmap level-of-detail number.
		 */
		virtual void set_image(const GraphicContextPtr &context, const PixelBufferPtr &image, int level = 0) = 0;

		/** Upload image to sub-texture.
		 *  \param context Graphic context to use for the request.
		 *  \param x       The horizontal point in the texture to write the new sub-texture image onto.
		 *  \param y       The vertical point in the texture to write the new sub-texture image onto.
		 *  \param point   Point in the texture to write the new sub-texture image onto.
		 *  \param image   Image to upload.
		 *  \param level   Mipmap level-of-detail number.
		 */
		virtual void set_subimage(const GraphicContextPtr &context, int x, int y, const PixelBufferPtr &image, const Rect &src_rect, int level = 0) = 0;

		/** Upload image to sub-texture.
		 *  \param context Graphic context to use for the request.
		 *  \param image   Image to upload.
		 *  \param level   Mipmap level-of-detail number.
		 */
		void set_subimage(const GraphicContextPtr &context, const Point &point, const PixelBufferPtr &image, const Rect &src_rect, int level = 0)
		{
			set_subimage(context, point.x, point.y, image, src_rect, level);
		}

		/// Copy image data from a graphic context.
		virtual void copy_image_from(const GraphicContextPtr &context, int level, TextureFormat texture_format = tf_rgba8) = 0;
		virtual void copy_image_from(const GraphicContextPtr &context, int x, int y, int width, int height, int level = 0, TextureFormat texture_format = tf_rgba8) = 0;
		void copy_image_from(const GraphicContextPtr &context, const Rect &pos, int level = 0, TextureFormat texture_format = tf_rgba8)
		{
			copy_image_from(context, pos.left, pos.top, pos.get_width(), pos.get_height(), level, texture_format);
		}

		/// Copy sub image data from a graphic context.
		virtual void copy_subimage_from(const GraphicContextPtr &context, int offset_x, int offset_y, int x, int y, int width, int height, int level = 0) = 0;
		virtual void copy_subimage_from(const GraphicContextPtr &context, const Point &offset, const Rect &pos, int level = 0)
		{
			copy_subimage_from(context, offset.x, offset.y, pos.left, pos.top, pos.get_width(), pos.get_height(), level);
		}

		virtual void set_wrap_mode(TextureWrapMode wrap_s, TextureWrapMode wrap_t) = 0;
	};
}
