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

#include "UICore/precomp.h"
#include <iostream>
#include "UICore/Core/IOData/file.h"
#include "UICore/Core/IOData/path_help.h"
#include "UICore/Display/ImageFormats/jpeg_format.h"
#include "UICore/Core/System/databuffer.h"
#include "UICore/Core/System/exception.h"
#include "UICore/Core/Text/text.h"
#include "JPEGLoader/jpeg_loader.h"
#include "JPEGWriter/jpge.h"

namespace uicore
{
	PixelBufferPtr JPEGFormat::load(const std::string &filename, bool srgb)
	{
		auto file = File::open_existing(filename);
		return JPEGLoader::load(file, srgb);
	}

	PixelBufferPtr JPEGFormat::load(const IODevicePtr &file, bool srgb)
	{
		return JPEGLoader::load(file, srgb);
	}

	void JPEGFormat::save(PixelBufferPtr buffer, const std::string &filename, int quality)
	{
		auto file = File::create_always(filename);
		return JPEGFormat::save(buffer, file, quality);
	}

	void JPEGFormat::save(PixelBufferPtr buffer, const IODevicePtr &file, int quality)
	{
		if (buffer->format() != tf_rgb8)
		{
			auto newbuf = PixelBuffer::create(buffer->width(), buffer->height(), tf_rgb8);
			newbuf->set_image(buffer);
			buffer = newbuf;
		}

		auto output = DataBuffer::create(buffer->width() * buffer->height() * 5);
		int size = output->size();

		uicore_jpge::params desc;
		desc.m_quality = quality;
		bool result = uicore_jpge::compress_image_to_jpeg_file_in_memory(output->data(), size, buffer->width(), buffer->height(), 3, output->data<uicore_jpge::uint8>(), desc);
		if (!result)
			throw Exception("Unable to compress JPEG image");

		file->write(output->data(), size);
	}
}
