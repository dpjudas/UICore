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

#include "UICore/Core/Math/vec4.h"
#include "UICore/Core/Math/half_float_vector.h"
#include <memory>
#include <vector>

namespace uicore
{
	class PixelReader
	{
	public:
		virtual ~PixelReader() { }
		virtual void read(const void *input, Vec4f *output, int num_pixels) = 0;
	};

	class PixelWriter
	{
	public:
		virtual ~PixelWriter() { }
		virtual void write(void *output, Vec4f *input, int num_pixels) = 0;
	};

	class PixelFilter
	{
	public:
		virtual ~PixelFilter() { }
		virtual void filter(Vec4f *pixels, int num_pixels) = 0;
	};

	class PixelConverterImpl : public PixelConverter
	{
	public:
		bool premultiply_alpha() const override { return _premultiply_alpha; }
		bool flip_vertical() const override { return _flip_vertical; }
		float gamma() const override { return _gamma; }
		Vec4i swizzle() const override { return _swizzle; }
		bool input_is_ycrcb() const override { return _input_is_ycrcb; }
		bool output_is_ycrcb() const override { return _output_is_ycrcb; }
		void set_premultiply_alpha(bool value) override { _premultiply_alpha = value; }
		void set_flip_vertical(bool value) override { _flip_vertical = value; }
		void set_gamma(float value) override { _gamma = value; }
		void set_swizzle(const Vec4i &value) override { _swizzle = value; }
		void set_input_is_ycrcb(bool value) override { _input_is_ycrcb = value; }
		void set_output_is_ycrcb(bool value) override { _output_is_ycrcb = value; }

		void convert(void *output, int output_pitch, TextureFormat output_format, const void *input, int input_pitch, TextureFormat input_format, int width, int height) override;

	private:
		std::unique_ptr<PixelReader> create_reader(TextureFormat format, bool sse2);
		std::unique_ptr<PixelWriter> create_writer(TextureFormat format, bool sse2, bool sse4);
		std::vector<std::shared_ptr<PixelFilter> > create_filters(bool sse2);

		bool _premultiply_alpha = false;
		bool _flip_vertical = false;
		float _gamma = 1.0f;
		Vec4i _swizzle = Vec4i(0, 1, 2, 3);
		bool _input_is_ycrcb = false;
		bool _output_is_ycrcb = false;
	};
}
