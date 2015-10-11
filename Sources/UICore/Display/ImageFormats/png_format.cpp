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

#include "UICore/precomp.h"
#include "UICore/Core/System/exception.h"
#include "UICore/Core/IOData/file.h"
#include "UICore/Core/IOData/path_help.h"
#include "UICore/Core/Text/text.h"
#include "UICore/Display/Image/pixel_buffer.h"
#include "UICore/Display/ImageFormats/png_format.h"
#include "UICore/Display/ImageFormats/PNGLoader/png_loader.h"
#include "UICore/Display/ImageFormats/PNGWriter/png_writer.h"
#include "UICore/Core/Zip/miniz.h"
#include <stdlib.h>

namespace uicore
{
	PixelBufferPtr PNGFormat::load(const std::string &filename, bool srgb)
	{
		auto file = File::open_existing(filename);
		return PNGLoader::load(file, srgb);
	}

	PixelBufferPtr PNGFormat::load(const IODevicePtr &file, bool srgb)
	{
		return PNGLoader::load(file, srgb);
	}

	void PNGFormat::save(PixelBufferPtr buffer, const std::string &filename)
	{
		auto file = File::create_always(filename);
		save(buffer, file);
	}

	void PNGFormat::save(PixelBufferPtr buffer, const IODevicePtr &iodev)
	{
		PNGWriter::save(iodev, buffer);
		/*
		if (buffer.get_format() != tf_rgba8)
		{
			PixelBuffer newbuf(
			buffer.get_width(),
			buffer.get_height(),
			tf_rgba8);
			newbuf.set_image(buffer);
			buffer = newbuf;
		}

		size_t size = 0;
		void *data = tdefl_write_image_to_png_file_in_memory(buffer.get_data(), buffer.get_width(), buffer.get_height(), 4, &size);
		try
		{
			iodev.write(data, size);
			if (data)
			free(data);
		}
		catch (...)
		{
			if (data)
			free(data);
			throw;
		}
		*/
	}
}
