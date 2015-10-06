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
**    Mark Page
*/

#pragma once

#include <memory>
#include "../../Core/Math/vec4.h"
#include "texture_format.h"

namespace uicore
{
	/// \brief Pixel format converter class.
	class PixelConverter
	{
	public:
		/// \brief Constructs a pixel format converter
		static std::shared_ptr<PixelConverter> create();

		/// \brief Returns the premultiply alpha setting
		virtual bool premultiply_alpha() const = 0;

		/// \brief Returns the flip vertical setting
		virtual bool flip_vertical() const = 0;

		/// \brief Returns the gamma setting
		virtual float gamma() const = 0;

		/// \brief Returns the input channel used for each output channel
		virtual Vec4i swizzle() const = 0;

		/// \brief Returns the JPEG JFIF YCrCb input setting
		virtual bool input_is_ycrcb() const = 0;

		/// \brief Returns the JPEG JFIF YCrCb output setting
		virtual bool output_is_ycrcb() const = 0;

		/// \brief Set the premultiply alpha setting
		///
		/// This defaults to off.
		virtual void set_premultiply_alpha(bool enable) = 0;

		/// \brief Set the flip vertical setting
		///
		/// This defaults to off.
		virtual void set_flip_vertical(bool enable) = 0;

		/// \brief Set the gamma applied when converting
		///
		/// This defaults to 1.0 (off).
		virtual void set_gamma(float gamma) = 0;

		/// \brief Set the input channel used for each output channel
		///
		/// Values 0-3 accepted. 0 = red, 1 = green, 2 = blue, 3 = alpha.
		virtual void set_swizzle(const Vec4i &swizzle) = 0;
		void set_swizzle(int red_source, int green_source, int blue_source, int alpha_source) { set_swizzle(Vec4i(red_source, green_source, blue_source, alpha_source)); }

		/// \brief Converts from JPEG JFIF YCrCb
		virtual void set_input_is_ycrcb(bool enable) = 0;

		/// \brief Converts to JPEG JFIF YCrCb
		virtual void set_output_is_ycrcb(bool enable) = 0;

		/// \brief Convert some pixel data
		virtual void convert(void *output, int output_pitch, TextureFormat output_format, const void *input, int input_pitch, TextureFormat input_format, int width, int height) = 0;
	};

	typedef std::shared_ptr<PixelConverter> PixelConverterPtr;
}
