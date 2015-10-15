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
**    Mark Page
*/

#pragma once

#include "UICore/Display/Image/pixel_buffer.h"

namespace uicore
{
	class PixelBuffer_Impl;

	class CPUPixelBufferProvider : public PixelBuffer
	{
	public:
		CPUPixelBufferProvider(TextureFormat new_format, const Size &new_size, const void *data_ptr, bool only_reference_data);
		~CPUPixelBufferProvider();

		void *data() override { return _data; }
		const void *data() const override { return _data; }
		int pitch() const override { return _width * bytes_per_pixel(texture_format); }
		int width() const override { return _width; }
		int height() const override { return _height; }
		TextureFormat format() const override { return texture_format; };

		float pixel_ratio() const override { return _pixel_ratio; }
		void set_pixel_ratio(float ratio) override { _pixel_ratio = ratio; }

	private:
		bool delete_data = true;
		unsigned char *_data = nullptr;

		int _width = 0;
		int _height = 0;
		TextureFormat texture_format;
		float _pixel_ratio = 0.0f;
	};
}
