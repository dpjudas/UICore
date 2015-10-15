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
**    Kenneth Gangstoe
*/

#pragma once

#include <memory>
#include "../../Core/Math/origin.h"
#include "../../Core/Math/color.h"
#include "../Image/image_import_description.h"
#include "UICore/Display/Render/texture.h"

namespace uicore
{
	class Rect;
	class Size;
	class Rectf;
	class Texture2D;
	typedef std::shared_ptr<Texture2D> Texture2DPtr;
	class TextureGroupImage;
	class PixelBuffer;
	class Canvas;
	typedef std::shared_ptr<Canvas> CanvasPtr;
	class Quadf;

	/// \brief Image class.
	class Image
	{
	public:
		/// \brief Constructs an image
		static std::shared_ptr<Image> create(Texture2DPtr texture, const Rect &rect, float pixel_ratio = 1.0f);
		static std::shared_ptr<Image> create(TextureGroupImage &sub_texture, float pixel_ratio = 1.0f);
		static std::shared_ptr<Image> create(const CanvasPtr &canvas, const PixelBufferPtr &pixelbuffer, const Rect &rect, float pixel_ratio = 1.0f);
		static std::shared_ptr<Image> create(const CanvasPtr &canvas, const std::string &filename, const ImageImportDescription &import_desc = ImageImportDescription(), float pixel_ratio = 1.0f);

		/// \brief Returns x scale.
		virtual float scale_x() const = 0;

		/// \brief Returns y scale.
		virtual float scale_y() const = 0;

		/// \brief Returns current color.
		virtual Colorf color() const = 0;

		/// \brief Returns translation hot-spot.
		virtual void get_alignment(Origin &origin, float &x, float &y) const = 0;

		/// \brief Return the texture of the image
		virtual TextureGroupImage texture() const = 0;

		/// \brief Return the size of the image.
		virtual Sizef size() const = 0;

		/// \brief Return the width of the image.
		virtual float width() const = 0;

		/// \brief Return the height of the image.
		virtual float height() const = 0;

		/// \brief Copies all information from this image to another, excluding the graphics that remain shared
		virtual std::shared_ptr<Image> clone() const = 0;

		/// \brief Draw image on graphic context.
		///
		/// \param canvas Target canvas
		/// \param x, y Anchor position of where to render image. Actual rendering position depends on the anchor and the alignment mode.
		/// \param src Source rectangle to draw
		/// \param dest Area to draw image in
		virtual void draw(const CanvasPtr &canvas, float x, float y) const = 0;
		virtual void draw(const CanvasPtr &canvas, const Rectf &src, const Rectf &dest) const = 0;
		virtual void draw(const CanvasPtr &canvas, const Rectf &dest) const = 0;
		virtual void draw(const CanvasPtr &canvas, const Rectf &src, const Quadf &dest) const = 0;
		virtual void draw(const CanvasPtr &canvas, const Quadf &dest) const = 0;

		/// \brief Set scale factors
		virtual void set_scale(float x, float y) = 0;

		/// \brief Sets the color.
		virtual void set_color(const Colorf &color) = 0;
		void set_color(const Color& c) { Colorf color; color.r = c.get_red() / 255.0f; color.g = c.get_green() / 255.0f; color.b = c.get_blue() / 255.0f; color.a = c.get_alpha() / 255.0f; set_color(color); }

		/// \brief Sets translation hotspot.
		virtual void set_alignment(Origin origin, float x = 0, float y = 0) = 0;

		/// \brief Sets the wrapping mode to use
		virtual void set_wrap_mode(TextureWrapMode wrap_s, TextureWrapMode wrap_t) = 0;

		/// \brief Set to true if a linear filter should be used for scaling up and down, false if a nearest-point filter should be used.
		virtual void set_linear_filter(bool linear_filter = true) = 0;
	};

	typedef std::shared_ptr<Image> ImagePtr;
}
