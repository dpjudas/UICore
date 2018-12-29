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
*/

#pragma once

#include <memory>
#include "../../Core/Math/rect.h"
#include "texture_format.h"

namespace uicore
{
	class PixelBuffer;

	/// \brief Set of images that combined form a complete texture
	class PixelBufferSet
	{
	public:
		/// \brief Constructs an image set of the specified dimensions type and internal format
		static std::shared_ptr<PixelBufferSet> create(TextureDimensions dimensions, TextureFormat format, int width, int height = 1, int slices = 1);

		/// \brief Constructs an image set with a single image using the dimensions and internal format of the pixel buffer
		static std::shared_ptr<PixelBufferSet> create(const std::shared_ptr<PixelBuffer> &image);

		/// \brief Returns the texture dimensions used by the image set
		virtual TextureDimensions dimensions() const = 0;

		/// \brief Returns the internal texture format used by the image
		virtual TextureFormat format() const = 0;

		/// \brief Returns the width of the image
		virtual int width() const = 0;

		/// \brief Returns the height of the image
		virtual int height() const = 0;

		/// \brief Returns the number of depth/array/cube slices in the set
		virtual int slice_count() const = 0;

		/// \brief Returns the lowest mip level specified in the set
		virtual int base_level() const = 0;

		/// \brief Returns the highest mip level specified in the set
		virtual int max_level() const = 0;

		/// \brief Returns the pixel buffer for a specific slice and level
		virtual std::shared_ptr<PixelBuffer> image(int slice, int level) = 0;

		/// \brief Set the pixel buffer to be used for the specified slice and level
		virtual void set_image(int slice, int level, const std::shared_ptr<PixelBuffer> &image) = 0;
	};
}
