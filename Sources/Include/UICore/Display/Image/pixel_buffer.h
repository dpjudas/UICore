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
**    Mark Page
*/

#pragma once

#include <memory>
#include "../../Core/Math/rect.h"
#include "texture_format.h"

namespace uicore
{
	class Size;
	class Rect;
	class PixelFormat;
	class Color;
	class Colorf;
	class PixelBuffer;
	class IODevice;
	class GraphicContext;
	typedef std::shared_ptr<GraphicContext> GraphicContextPtr;
	class PixelConverter;
	typedef std::shared_ptr<PixelConverter> PixelConverterPtr;

	/// \brief Pixel data container.
	class PixelBuffer
	{
	public:
		/// \brief Constructs a PixelBuffer
		///
		/// \param width = value
		/// \param height = value
		/// \param sized_format = Pixel Format
		/// \param data = The data (0 = Allocate automatically to a boundary of 16 bytes)
		/// \param only_reference_data : true = Reference the data. false = Copy the data
		static std::shared_ptr<PixelBuffer> create(int width, int height, TextureFormat texture_format, const void *data = nullptr, bool only_reference_data = false);

		/// \brief Create a copy of the pixelbuffer that doesn't share data with the original pixel buffer.
		std::shared_ptr<PixelBuffer> copy() const;
		std::shared_ptr<PixelBuffer> copy(const Rect &rect) const;

		/// Retrieves the width of the buffer.
		virtual int width() const = 0;

		/// Retrieves the height of the buffer.
		virtual int height() const = 0;

		/// Retrieves the size of the buffer.
		Size size() const { return Size{ width(), height() }; }

		/// Returns the pitch (bytes allocated per row).
		virtual int pitch() const = 0;

		/** Retrieves the pixel ratio of this image.
		 *  \return The display pixel ratio set for this image.
		 *          A zero value implies that no pixel ratio has been set
		 */
		virtual float pixel_ratio() const = 0;

		/// Returns the device independent width of this texture.
		float dip_width() const { return width() / pixel_ratio(); }

		/// Returns the device independent height of this texture.
		float dip_height() const { return height() / pixel_ratio(); }

		/// Returns the device independent size of this texture.
		Sizef dip_size() const { return Sizef{ dip_width(), dip_height() }; }

		/// \brief Returns a pointer to the beginning of the pixel buffer.
		virtual void *data() = 0;
		virtual const void *data() const = 0;

		template<typename Type> Type *data() { return reinterpret_cast<Type*>(data()); }
		template<typename Type> const Type *data() const { return reinterpret_cast<const Type*>(data()); }

		/// \brief Returns a pointer to the beginning of the pixel buffer as 8 bit data.
		unsigned char *data_uint8() { return reinterpret_cast<unsigned char*>(data()); }
		const unsigned char *data_uint8() const { return reinterpret_cast<const unsigned char*>(data()); }

		/// \brief Returns a pointer to the beginning of the pixel buffer as 16 bit data.
		unsigned short *data_uint16() { return reinterpret_cast<unsigned short*>(data()); }
		const unsigned short *data_uint16() const { return reinterpret_cast<const unsigned short*>(data()); }

		/// \brief Returns a pointer to the beginning of the pixel buffer as 32 bit data.
		unsigned int *data_uint32() { return reinterpret_cast<unsigned int*>(data()); }
		const unsigned int *data_uint32() const { return reinterpret_cast<const unsigned int*>(data()); }

		/// \brief Returns a pointer to the beginning of a specific line.
		void *line(int line) { unsigned char *d = data_uint8(); return d + line * pitch(); }
		const void *line(int line) const { const unsigned char *d = data_uint8(); return d + line * pitch(); }

		template<typename T> T *line(int line) { unsigned char *d = data_uint8(); return reinterpret_cast<T*>(d + line * pitch()); }
		template<typename T> const T *line(int line) const { const unsigned char *d = data_uint8(); return reinterpret_cast<const T*>(d + line * pitch()); }

		/// \brief Returns a pointer to the beginning of a specific line as 8 bit data.
		unsigned char *line_uint8(int index) { return reinterpret_cast<unsigned char*>(line(index)); }
		const unsigned char *line_uint8(int index) const { return reinterpret_cast<const unsigned char*>(line(index)); }

		/// \brief Returns a pointer to the beginning of a specific line as 16 bit data.
		unsigned short *line_uint16(int index) { return reinterpret_cast<unsigned short*>(line(index)); }
		const unsigned short *line_uint16(int index) const { return reinterpret_cast<const unsigned short*>(line(index)); }

		/// \brief Returns a pointer to the beginning of a specific line as 32 bit data.
		unsigned int *line_uint32(int index) { return reinterpret_cast<unsigned int*>(line(index)); }
		const unsigned int *line_uint32(int index) const { return reinterpret_cast<const unsigned int*>(line(index)); }

		/// \brief Returns true if format has an alpha channel
		bool has_transparency() const;

		/// \brief Returns the number of bytes per pixel
		///
		/// \return Bytes per pixel. Exception thrown if not available (hint, use is_compressed() )
		unsigned int bytes_per_pixel() const;

		/// \brief Returns the number of bytes per compression block
		///
		/// \return Bytes per block. Exception thrown if not available (hint, use is_compressed() )
		unsigned int bytes_per_block() const;

		/// \brief Returns the size in bytes of the image data
		///
		/// \return The data size
		unsigned int data_size() const;

		/// \brief Returns the size in bytes of the image data
		///
		/// \return The data size
		static unsigned int data_size(const Size &size, TextureFormat texture_format);

		/// \brief Returns the number of bytes per pixel
		///
		/// \return Bytes per pixel. Exception thrown if not available (hint, use is_compressed() )
		static unsigned int bytes_per_pixel(TextureFormat texture_format);

		/// \brief Returns the number of bytes per compression block
		///
		/// \return Bytes per block. Exception thrown if not available (hint, use is_compressed() )
		static unsigned int bytes_per_block(TextureFormat texture_format);

		/// \brief Returns true if compressed
		bool is_compressed() const;

		/// \brief Tests if the specified texture format is a compressed format
		static bool is_compressed(TextureFormat texture_format);

		/// \brief Returns the pixel format
		virtual TextureFormat format() const = 0;

		/// \brief Copy source pixel buffer into this buffer, doing a format conversion if needed
		///
		/// \param source Source pixel buffer.
		void set_image(const std::shared_ptr<PixelBuffer> &source);

		/// \brief Copy source pixel buffer into this buffer, doing a format conversion if needed
		///
		/// \param source Source pixel buffer.
		void set_image(const std::shared_ptr<PixelBuffer> &source, const PixelConverterPtr &converter);

		/// \brief Copy source pixel buffer into this buffer, doing a format conversion if needed
		///
		/// \param source Source pixel buffer.
		/// \param dest_rect Destination position for copy.
		/// \param src_rect Source rectangle for copy.
		void set_subimage(const std::shared_ptr<PixelBuffer> &source, const Point &dest_pos, const Rect &src_rect);

		/// \brief Copy source pixel buffer into this buffer, doing a format conversion if needed
		///
		/// \param source Source pixel buffer.
		/// \param dest_rect Destination position for copy.
		/// \param src_rect Source rectangle for copy.
		void set_subimage(const std::shared_ptr<PixelBuffer> &source, const Point &dest_pos, const Rect &src_rect, const PixelConverterPtr &converter);

		/// \brief Converts current buffer to a new pixel format and returns the result.
		std::shared_ptr<PixelBuffer> to_format(TextureFormat texture_format) const;

		/// \brief Converts current buffer to a new pixel format and returns the result.
		std::shared_ptr<PixelBuffer> to_format(TextureFormat texture_format, const PixelConverterPtr &converter) const;

		/// \brief Flip the entire image vertically (turn it upside down)
		void flip_vertical();

		/// \brief Multiply the RGB components by the Alpha component
		///
		/// This is useful with certain blending functions
		void premultiply_alpha();

		/// \brief Multiply the RGB components by gamma value
		///
		/// Calling this function with 2.2 gamma converts a sRGB image into linear space.
		/// To convert from linear to sRGB use 1.0/2.2
		void premultiply_gamma(float gamma);

		/// Sets the display pixel ratio for this image.
		virtual void set_pixel_ratio(float ratio) = 0;

		/// \brief Add a border around a pixelbuffer, duplicating the edge pixels
		static std::shared_ptr<PixelBuffer> add_border(const std::shared_ptr<PixelBuffer> &pb, int border_size, const Rect &rect);
	};

	typedef std::shared_ptr<PixelBuffer> PixelBufferPtr;
}
