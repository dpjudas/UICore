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

#include "UICore/precomp.h"
#include "UICore/Display/Image/pixel_buffer.h"
#include "UICore/Display/Image/pixel_converter.h"
#include "UICore/Display/2D/color.h"
#include "UICore/Core/System/exception.h"
#include "UICore/Core/IOData/path_help.h"
#include "UICore/Display/ImageFormats/image_file.h"
#include "UICore/Display/Render/transfer_texture.h"
#include "UICore/Core/Text/string_format.h"
#include "UICore/Core/Text/text.h"
#include "UICore/Display/Image/texture_format.h"
#include "UICore/Core/Math/half_float.h"
#include "UICore/Core/Math/half_float_vector.h"
#include "cpu_pixel_buffer_provider.h"
#include <cstdint>

namespace uicore
{
	class PixelBufferImpl
	{
	public:
		static void convert(const PixelBuffer *source, PixelBuffer *target, const Rect &dest_rect, const Rect &src_rect, const PixelConverterPtr &converter)
		{
			if (dest_rect.get_size() != src_rect.get_size())
			{
				throw Exception("Source and destination rects must have same size. Scaled converting not supported.");
			}

			const char *src_data = source->data<char>();
			char *dest_data = target->data<char>();

			int src_pitch = source->width() * source->bytes_per_pixel();
			int dest_pitch = target->width() * target->bytes_per_pixel();

			src_data += src_rect.top * src_pitch + src_rect.left * source->bytes_per_pixel();
			dest_data += dest_rect.top * dest_pitch + dest_rect.left * target->bytes_per_pixel();

			converter->convert(dest_data, dest_pitch, target->format(), src_data, src_pitch, source->format(), dest_rect.get_width(), dest_rect.get_height());
		}
	};

	std::shared_ptr<PixelBuffer> PixelBuffer::create(int width, int height, TextureFormat texture_format, const void *data, bool only_reference_data)
	{
		return std::make_shared<CPUPixelBufferProvider>(texture_format, Size(width, height), data, only_reference_data);
	}

	bool PixelBuffer::has_transparency() const
	{
		switch (format())
		{
		case tf_rgba8:
		case tf_bgra8:
		case tf_rgba2:
		case tf_rgba4:
		case tf_rgb5_a1:
		case tf_rgba8_snorm:
		case tf_rgb10_a2:
		case tf_rgba12:
		case tf_rgba16:
		case tf_rgba16_snorm:
		case tf_srgb8_alpha8:
		case tf_rgba32f:
		case tf_rgba8i:
		case tf_rgba8ui:
		case tf_rgba16i:
		case tf_rgba16ui:
		case tf_rgba32i:
		case tf_rgba32ui:
		case tf_compressed_rgba:
		case tf_compressed_srgb_alpha:
		case tf_compressed_rgba_s3tc_dxt1:
		case tf_compressed_rgba_s3tc_dxt3:
		case tf_compressed_rgba_s3tc_dxt5:
		case tf_compressed_srgb_alpha_s3tc_dxt1:
		case tf_compressed_srgb_alpha_s3tc_dxt3:
		case tf_compressed_srgb_alpha_s3tc_dxt5:
			return true;

		case tf_rgb8:
		case tf_bgr8:
		case tf_stencil_index1:
		case tf_stencil_index4:
		case tf_stencil_index8:
		case tf_stencil_index16:
		case tf_r8:
		case tf_r8_snorm:
		case tf_r16:
		case tf_r16_snorm:
		case tf_rg8:
		case tf_rg8_snorm:
		case tf_rg16:
		case tf_rg16_snorm:
		case tf_r3_g3_b2:
		case tf_rgb4:
		case tf_rgb5:
		case tf_rgb8_snorm:
		case tf_rgb10:
		case tf_rgb12:
		case tf_rgb16:
		case tf_rgb16_snorm:
		case tf_srgb8:
		case tf_r16f:
		case tf_rg16f:
		case tf_rgb16f:
		case tf_rgba16f:
		case tf_r32f:
		case tf_rg32f:
		case tf_rgb32f:
		case tf_r11f_g11f_b10f:
		case tf_rgb9_e5:
		case tf_r8i:
		case tf_r8ui:
		case tf_r16i:
		case tf_r16ui:
		case tf_r32i:
		case tf_r32ui:
		case tf_rg8i:
		case tf_rg8ui:
		case tf_rg16i:
		case tf_rg16ui:
		case tf_rg32i:
		case tf_rg32ui:
		case tf_rgb8i:
		case tf_rgb8ui:
		case tf_rgb16i:
		case tf_rgb16ui:
		case tf_rgb32i:
		case tf_rgb32ui:
		case tf_depth_component16:
		case tf_depth_component24:
		case tf_depth_component32:
		case tf_depth_component32f:
		case tf_depth24_stencil8:
		case tf_depth32f_stencil8:
		case tf_compressed_red:
		case tf_compressed_rg:
		case tf_compressed_rgb:
		case tf_compressed_srgb:
		case tf_compressed_red_rgtc1:
		case tf_compressed_signed_red_rgtc1:
		case tf_compressed_rg_rgtc2:
		case tf_compressed_signed_rg_rgtc2:
		case tf_compressed_rgb_s3tc_dxt1:
		case tf_compressed_srgb_s3tc_dxt1:
			return false;

		default:
			throw Exception("Unknown pixel buffer format encountered in PixelBuffer::has_transparency");
		}
	}

	unsigned int PixelBuffer::bytes_per_pixel() const
	{
		return bytes_per_pixel(format());
	}

	unsigned int PixelBuffer::bytes_per_pixel(TextureFormat texture_format)
	{
		unsigned int count;
		switch (texture_format)
		{
		case tf_r8: count = 8;	break; //RED 8
		case tf_r8_snorm: count = 8; break; //RED, s8
		case tf_r16: count = 16; break; //RED, 16
		case tf_r16_snorm: count = 16; break; //RED, s16
		case tf_rg8: count = 8 + 8; break; //RG, 8, 8
		case tf_rg8_snorm: count = 8 + 8; break; //RG, s8, s8
		case tf_rg16: count = 16 + 16; break; //RG, 16, 16
		case tf_rg16_snorm: count = 16 + 16; break; //RG, s16, s16
		case tf_r3_g3_b2: count = 3 + 3 + 2; break; //RGB, 3, 3, 2
		case tf_rgb4: count = 4 + 4 + 4; break; //RGB, 4, 4, 4
		case tf_rgb5: count = 5 + 5 + 5; break; //RGB, 5, 5, 5
		case tf_rgb8: count = 8 + 8 + 8; break; //RGB, 8, 8, 8
		case tf_bgr8: count = 8 + 8 + 8; break; //BGR, 8, 8, 8
		case tf_rgb8_snorm: count = 8 + 8 + 8; break; //RGB, s8, s8, s8
		case tf_rgb10: count = 10 + 10 + 10; break; //RGB, 10, 10, 10
		case tf_rgb12: count = 12 + 12 + 12; break; //RGB, 12, 12, 12
		case tf_rgb16: count = 16 + 16 + 16; break; //RGB, 16, 16, 16
		case tf_rgb16_snorm: count = 16 + 16 + 16; break; //RGB, s16, s16, s16
		case tf_rgba2: count = 2 + 2 + 2 + 2; break; //RGBA, 2, 2, 2, 2
		case tf_rgba4: count = 4 + 4 + 4 + 4; break; //RGBA, 4, 4, 4, 4
		case tf_rgb5_a1: count = 5 + 5 + 5 + 1; break; //RGBA, 5, 5, 5, 1
		case tf_rgba8: count = 8 + 8 + 8 + 8; break; //RGBA, 8, 8, 8, 8
		case tf_rgba8_snorm: count = 8 + 8 + 8 + 8; break; //RGBA, s8, s8, s8, s8
		case tf_bgra8: count = 8 + 8 + 8 + 8; break; //BGRA, 8, 8, 8, 8
		case tf_rgb10_a2: count = 10 + 10 + 10 + 2; break; //RGBA, 10, 10, 10, 2
		case tf_rgba12: count = 12 + 12 + 12 + 12; break; //RGBA, 12, 12, 12, 12
		case tf_rgba16: count = 16 + 16 + 16 + 16; break; //RGBA, 16, 16, 16, 16
		case tf_rgba16_snorm: count = 16 + 16 + 16 + 16; break; //RGBA, s16, s16, s16, s16
		case tf_srgb8: count = 8 + 8 + 8; break; //RGB, 8, 8, 8
		case tf_srgb8_alpha8: count = 8 + 8 + 8 + 8; break; //RGBA, 8, 8, 8, 8
		case tf_r16f: count = 16; break; //RED, f16
		case tf_rg16f: count = 16 + 16; break; //RG, f16, f16
		case tf_rgb16f: count = 16 + 16 + 16; break; //RGB, f16, f16, f16
		case tf_rgba16f: count = 16 + 16 + 16 + 16; break; //RGBA, f16, f16, f16, f16
		case tf_r32f: count = 32; break; //RED, f32
		case tf_rg32f: count = 32 + 32; break; //RG, f32, f32
		case tf_rgb32f: count = 32 + 32 + 32; break; //RGB, f32, f32, f32
		case tf_rgba32f: count = 32 + 32 + 32 + 32; break; //RGBA, f32, f32, f32, f32
		case tf_r11f_g11f_b10f: count = 11 + 11 + 10; break; //RGB, f11, f11, f10
		case tf_rgb9_e5: count = 9 + 9 + 9 + 5; break; //RGB, 9, 9, 9, 5
		case tf_r8i: count = 8; break; //RED, i8
		case tf_r8ui: count = 8; break; //RED, ui8
		case tf_r16i: count = 16; break; //RED, i16
		case tf_r16ui: count = 16; break; //RED, ui16
		case tf_r32i: count = 32; break; //RED, i32
		case tf_r32ui: count = 32; break; //RED, ui32
		case tf_rg8i: count = 8 + 8; break; //RG, i8, i8
		case tf_rg8ui: count = 8 + 8; break; //RG, ui8, ui8
		case tf_rg16i: count = 16 + 16; break; //RG, i16, i16
		case tf_rg16ui: count = 16 + 16; break; //RG, ui16, ui16
		case tf_rg32i: count = 32 + 32; break; //RG, i32, i32
		case tf_rg32ui: count = 32 + 32; break; //RG, ui32, ui32
		case tf_rgb8i: count = 8 + 8 + 8; break; //RGB, i8, i8, i8
		case tf_rgb8ui: count = 8 + 8 + 8; break; //RGB, ui8, ui8, ui8
		case tf_rgb16i: count = 16 + 16 + 16; break; //RGB, i16, i16, i16
		case tf_rgb16ui: count = 16 + 16 + 16; break; //RGB, ui16, ui16, ui16
		case tf_rgb32i: count = 32 + 32 + 32; break; //RGB, i32, i32, i32
		case tf_rgb32ui: count = 32 + 32 + 32; break; //RGB, ui32, ui32, ui32
		case tf_rgba8i: count = 8 + 8 + 8 + 8; break; //RGBA, i8, i8, i8, i8
		case tf_rgba8ui: count = 8 + 8 + 8 + 8; break; //RGBA, ui8, ui8, ui8, ui8
		case tf_rgba16i: count = 16 + 16 + 16 + 16; break; //RGBA, i16, i16, i16, i16
		case tf_rgba16ui: count = 16 + 16 + 16 + 16; break; //RGBA, ui16, ui16, ui16, ui16
		case tf_rgba32i: count = 32 + 32 + 32 + 32; break; //RGBA, i32, i32, i32, i32
		case tf_rgba32ui: count = 32 + 32 + 32 + 32; break; //RGBA, ui32, ui32, ui32, ui32
		case tf_depth_component16: count = 16; break; //DEPTH_COMPONENT, 16
		case tf_depth_component24: count = 24; break; //DEPTH_COMPONENT, 24
		case tf_depth_component32: count = 32; break; //DEPTH_COMPONENT, 32
		case tf_depth_component32f: count = 32; break; //DEPTH_COMPONENT, f32
		case tf_depth24_stencil8: count = 24 + 8; break; //DEPTH_STENCIL, 24, 8
		case tf_depth32f_stencil8: count = 32 + 8; break; //DEPTH_STENCIL, f32, 8
		default:
			throw Exception("cannot obtain pixel count for this TextureFormat");
		}

		return (count + 7) / 8;
	}

	unsigned int PixelBuffer::bytes_per_block() const
	{
		return bytes_per_block(format());
	}

	unsigned int PixelBuffer::bytes_per_block(TextureFormat texture_format)
	{
		switch (texture_format)
		{
		case tf_compressed_rgb_s3tc_dxt1:
		case tf_compressed_rgba_s3tc_dxt1:
		case tf_compressed_rgba_s3tc_dxt3:
		case tf_compressed_srgb_s3tc_dxt1:
		case tf_compressed_srgb_alpha_s3tc_dxt1:
		case tf_compressed_srgb_alpha_s3tc_dxt3:
			return 8;
		case tf_compressed_rgba_s3tc_dxt5:
		case tf_compressed_srgb_alpha_s3tc_dxt5:
			return 16;
		default:
			throw Exception("cannot obtain block count for this TextureFormat");
		}
	}

	unsigned int PixelBuffer::data_size() const
	{
		return data_size(size(), format());
	}

	unsigned int PixelBuffer::data_size(const Size &size, TextureFormat texture_format)
	{
		if (is_compressed(texture_format))
		{
			return ((size.width + 3) / 4) * ((size.height + 3) / 4) * bytes_per_block(texture_format);
		}
		else
		{
			return size.width * size.height * bytes_per_pixel(texture_format);
		}
	}

	bool PixelBuffer::is_compressed() const
	{
		return is_compressed(format());
	}

	bool PixelBuffer::is_compressed(TextureFormat texture_format)
	{
		switch (texture_format)
		{
		case tf_compressed_rgb_s3tc_dxt1:
		case tf_compressed_rgba_s3tc_dxt1:
		case tf_compressed_rgba_s3tc_dxt3:
		case tf_compressed_srgb_s3tc_dxt1:
		case tf_compressed_srgb_alpha_s3tc_dxt1:
		case tf_compressed_srgb_alpha_s3tc_dxt3:
		case tf_compressed_rgba_s3tc_dxt5:
		case tf_compressed_srgb_alpha_s3tc_dxt5:
			return true;
		default:
			return false;
		}
	}

	std::shared_ptr<PixelBuffer> PixelBuffer::copy() const
	{
		auto pbuf = PixelBuffer::create(width(), height(), format());
		void *dst_data = pbuf->data();
		const void *src_data = data();
		memcpy(dst_data, src_data, height()*pitch());
		return pbuf;
	}

	std::shared_ptr<PixelBuffer> PixelBuffer::copy(const Rect &rect) const
	{
		if (rect.left < 0 || rect.top < 0 || rect.right > width() || rect.bottom > height())
			throw Exception("Rectangle passed to PixelBuffer::copy() out of bounds");

		int new_width = rect.get_width();
		int new_height = rect.get_height();

		auto pbuf = PixelBuffer::create(new_width, new_height, format());
		uint8_t *dst_data = pbuf->data<uint8_t>();
		const uint8_t *src_data = data<uint8_t>() + (rect.top * width() + rect.left) * bytes_per_pixel();

		int byte_width = new_width * bytes_per_pixel();
		int src_pitch = pitch();
		int dest_pitch = pbuf->pitch();
		for (int y_cnt = 0; y_cnt < new_height; y_cnt++)
		{
			memcpy(dst_data, src_data, byte_width);
			dst_data += dest_pitch;
			src_data += src_pitch;
		}
		return pbuf;
	}

	void PixelBuffer::set_image(const std::shared_ptr<PixelBuffer> &source)
	{
		set_subimage(source, Point(0, 0), Rect(Point(0, 0), source->size()));
	}

	void PixelBuffer::set_subimage(const std::shared_ptr<PixelBuffer> &source, const Point &dest_pos, const Rect &src_rect)
	{
		auto converter = PixelConverter::create();
		PixelBufferImpl::convert(source.get(), this, Rect(dest_pos, src_rect.get_size()), src_rect, converter);
	}

	void PixelBuffer::set_image(const std::shared_ptr<PixelBuffer> &source, const PixelConverterPtr &converter)
	{
		set_subimage(source, Point(0, 0), Rect(Point(0, 0), source->size()), converter);
	}

	void PixelBuffer::set_subimage(const std::shared_ptr<PixelBuffer> &source, const Point &dest_pos, const Rect &src_rect, const PixelConverterPtr &converter)
	{
		PixelBufferImpl::convert(source.get(), this, Rect(dest_pos, src_rect.get_size()), src_rect, converter);
	}

	std::shared_ptr<PixelBuffer> PixelBuffer::to_format(TextureFormat texture_format) const
	{
		auto converter = PixelConverter::create();
		return to_format(texture_format, converter);
	}

	std::shared_ptr<PixelBuffer> PixelBuffer::to_format(TextureFormat texture_format, const PixelConverterPtr &converter) const
	{
		auto result = PixelBuffer::create(width(), height(), texture_format);
		PixelBufferImpl::convert(this, result.get(), Rect(Point(), size()), Rect(Point(), size()), converter);
		return result;
	}

	void PixelBuffer::flip_vertical()
	{
		if (width() == 0 || height() <= 1)
			return;

		unsigned int line_pitch = pitch();
		std::vector<unsigned char> line_buffer;
		line_buffer.resize(line_pitch);

		int num_lines = (height() - 1) / 2;

		int start_offset = 0;
		int end_offset = (height() - 1) * line_pitch;

		char *d = data<char>();

		for (int cnt = 0; cnt < num_lines; cnt++, start_offset += line_pitch, end_offset -= line_pitch)
		{
			memcpy(&line_buffer[0], &d[start_offset], line_pitch);
			memcpy(&d[start_offset], &d[end_offset], line_pitch);
			memcpy(&d[end_offset], &line_buffer[0], line_pitch);
		}
	}

	void PixelBuffer::premultiply_alpha()
	{
		if (has_transparency())
		{
			if (format() == tf_rgba8 || format() == tf_srgb8_alpha8)
			{
				int w = width();
				int h = height();
				uint32_t *p = data<uint32_t>();
				for (int y = 0; y < h; y++)
				{
					int index = y * w;
					uint32_t *line = p + index;
					for (int x = 0; x < w; x++)
					{
						uint32_t a = ((line[x] >> 24) & 0xff);
						uint32_t b = ((line[x] >> 16) & 0xff);
						uint32_t g = ((line[x] >> 8) & 0xff);
						uint32_t r = (line[x] & 0xff);

						r = r * a / 255;
						g = g * a / 255;
						b = b * a / 255;

						line[x] = (a << 24) + (b << 16) + (g << 8) + r;
					}
				}
			}
			else if (format() == tf_bgra8)
			{
				int w = width();
				int h = height();
				uint32_t *p = data<uint32_t>();
				for (int y = 0; y < h; y++)
				{
					int index = y * w;
					uint32_t *line = p + index;
					for (int x = 0; x < w; x++)
					{
						uint32_t a = ((line[x] >> 24) & 0xff);
						uint32_t r = ((line[x] >> 16) & 0xff);
						uint32_t g = ((line[x] >> 8) & 0xff);
						uint32_t b = (line[x] & 0xff);

						r = r * a / 255;
						g = g * a / 255;
						b = b * a / 255;

						line[x] = (a << 24) + (r << 16) + (g << 8) + b;
					}
				}
			}
			else if (format() == tf_rgba16)
			{
				int w = width();
				int h = height();
				uint16_t *p = data<uint16_t>();
				for (int y = 0; y < h; y++)
				{
					int index = y * w;
					uint16_t *line = p + index;
					for (int x = 0; x < w; x++)
					{
						uint32_t r = line[x * 4];
						uint32_t g = line[x * 4 + 1];
						uint32_t b = line[x * 4 + 2];
						uint32_t a = line[x * 4 + 3];

						r = r * a / 65535;
						g = g * a / 65535;
						b = b * a / 65535;

						line[x * 4] = r;
						line[x * 4 + 1] = g;
						line[x * 4 + 2] = b;
					}
				}
			}
			else if (format() == tf_rgba16f)
			{
				int w = width();
				int h = height();
				unsigned short *p = data<unsigned short>();
				for (int y = 0; y < h; y++)
				{
					int index = y * w;
					unsigned short *line = p + index;
					for (int x = 0; x < w; x++)
					{
						float r = HalfFloat::half_to_float(line[x * 4]);
						float g = HalfFloat::half_to_float(line[x * 4 + 1]);
						float b = HalfFloat::half_to_float(line[x * 4 + 2]);
						float a = HalfFloat::half_to_float(line[x * 4 + 3]);

						r = r * a;
						g = g * a;
						b = b * a;

						line[x * 4] = HalfFloat::float_to_half(r);
						line[x * 4 + 1] = HalfFloat::float_to_half(g);
						line[x * 4 + 2] = HalfFloat::float_to_half(b);
					}
				}
			}
			else if (format() == tf_rgba32f)
			{
				int w = width();
				int h = height();
				float *p = data<float>();
				for (int y = 0; y < h; y++)
				{
					int index = y * w;
					float *line = p + index;
					for (int x = 0; x < w; x++)
					{
						float r = line[x * 4];
						float g = line[x * 4 + 1];
						float b = line[x * 4 + 2];
						float a = line[x * 4 + 3];

						r = r * a;
						g = g * a;
						b = b * a;

						line[x * 4] = r;
						line[x * 4 + 1] = g;
						line[x * 4 + 2] = b;
					}
				}
			}
			else
			{
				throw Exception("premultiply_alpha does not support the image format");
			}
		}
	}

	void PixelBuffer::premultiply_gamma(float gamma)
	{
		if (format() == tf_rgba8 || format() == tf_srgb8_alpha8 || format() == tf_bgra8)
		{
			int w = width();
			int h = height();
			for (int y = 0; y < h; y++)
			{
				Vec4ub *pixels = line<Vec4ub>(y);
				for (int x = 0; x < w; x++)
				{
					const float rcp_255 = 1.0f / 255.0f;
					float red = std::pow(pixels[x].r * rcp_255, gamma);
					float green = std::pow(pixels[x].g * rcp_255, gamma);
					float blue = std::pow(pixels[x].b * rcp_255, gamma);
					pixels[x].r = static_cast<unsigned short>(clamp(red * 255.0f + 0.5f, 0.0f, 255.0f));
					pixels[x].g = static_cast<unsigned short>(clamp(green * 255.0f + 0.5f, 0.0f, 255.0f));
					pixels[x].b = static_cast<unsigned short>(clamp(blue * 255.0f + 0.5f, 0.0f, 255.0f));
				}
			}
		}
		else if (format() == tf_rgba16)
		{
			int w = width();
			int h = height();
			for (int y = 0; y < h; y++)
			{
				Vec4us *pixels = line<Vec4us>(y);
				for (int x = 0; x < w; x++)
				{
					const float rcp_65535 = 1.0f / 65535.0f;
					float red = std::pow(pixels[x].r * rcp_65535, gamma);
					float green = std::pow(pixels[x].g * rcp_65535, gamma);
					float blue = std::pow(pixels[x].b * rcp_65535, gamma);
					pixels[x].r = static_cast<unsigned short>(clamp(red * 65535.0f + 0.5f, 0.0f, 65535.0f));
					pixels[x].g = static_cast<unsigned short>(clamp(green * 65535.0f + 0.5f, 0.0f, 65535.0f));
					pixels[x].b = static_cast<unsigned short>(clamp(blue * 65535.0f + 0.5f, 0.0f, 65535.0f));
				}
			}
		}
		else if (format() == tf_rgba16f)
		{
			int w = width();
			int h = height();
			for (int y = 0; y < h; y++)
			{
				Vec4hf *pixels = line<Vec4hf>(y);
				for (int x = 0; x < w; x++)
				{
					Vec4f v = pixels[x].to_float();
					v.r = std::pow(v.r, gamma);
					v.g = std::pow(v.g, gamma);
					v.b = std::pow(v.b, gamma);
					pixels[x] = Vec4hf(v);
				}
			}
		}
		else if (format() == tf_rgba32f)
		{
			int w = width();
			int h = height();
			for (int y = 0; y < h; y++)
			{
				Vec4f *pixels = line<Vec4f>(y);
				for (int x = 0; x < w; x++)
				{
					pixels[x].r = std::pow(pixels[x].r, gamma);
					pixels[x].g = std::pow(pixels[x].g, gamma);
					pixels[x].b = std::pow(pixels[x].b, gamma);
				}
			}
		}
	}

	PixelBufferPtr PixelBuffer::add_border(const PixelBufferPtr &pb, int border_size, const Rect &rect)
	{
		if (rect.left < 0 || rect.top < 0 || rect.right > pb->width() || rect.bottom > pb->height())
			throw Exception("Rectangle passed to PixelBuffer::add_border() out of bounds");

		if (border_size < 0)
		{
			border_size = 0;
		}

		int old_width = pb->width();
		int old_height = pb->height();

		int new_width = rect.get_width() + border_size * 2;
		int new_height = rect.get_height() + border_size * 2;

		// Convert pixel buffer if in an unsupported format
		PixelBufferPtr work_buffer;
		PixelBufferPtr work_pb = pb;
		if (work_pb->format() != tf_rgba8)
		{
			work_buffer = pb->to_format(tf_rgba8);
			work_pb = work_buffer;
		}

		auto new_pb = PixelBuffer::create(new_width, new_height, work_pb->format(), nullptr);

		int old_pitch = work_pb->pitch();
		int new_pitch = new_pb->pitch();

		int32_t *actual_src_data = (int32_t *)work_pb->data();
		actual_src_data += (rect.top * old_pitch) / 4;
		actual_src_data += rect.left;

		int32_t *actual_dest_data = (int32_t *)new_pb->data();

		for (int ypos = 0; ypos < new_height; ypos++)
		{
			int real_ypos = ypos - border_size;
			if (real_ypos < 0)
				real_ypos = 0;

			if (real_ypos >= old_height)
				real_ypos = old_height - 1;

			int32_t *src_data = actual_src_data;
			src_data += (old_pitch * real_ypos) / 4;

			int32_t *dest_data = actual_dest_data;
			dest_data += (new_pitch * ypos) / 4;

			for (int xpos = 0; xpos < new_width; xpos++)
			{
				int real_xpos = xpos - border_size;
				if (real_xpos < 0)
					real_xpos = 0;

				if (real_xpos >= old_width)
					real_xpos = old_width - 1;

				dest_data[xpos] = src_data[real_xpos];
			}
		}
		return new_pb;
	}
}
