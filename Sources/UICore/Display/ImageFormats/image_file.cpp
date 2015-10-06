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
**    Mark Page
*/

#include "UICore/precomp.h"
#include "UICore/Core/IOData/path_help.h"
#include "UICore/Core/IOData/file.h"
#include "UICore/Display/ImageFormats/image_file.h"
#include "UICore/Display/ImageFormats/image_file_type.h"
#include "UICore/Display/Image/pixel_buffer.h"
#include "UICore/Core/System/exception.h"
#include "UICore/Core/Text/text.h"
#include "UICore/Core/IOData/path_help.h"
#include "../setup_display.h"

namespace uicore
{
	PixelBufferPtr ImageFile::try_load(const std::string &filename, const std::string &type, std::string *out_failure_reason, bool srgb)
	{
		SetupDisplay::start();
		try
		{
			return load(filename, type, srgb);
		}
		catch (const Exception& e)
		{
			if (out_failure_reason)
				*out_failure_reason = e.message;
			return PixelBufferPtr();
		}
	}

	PixelBufferPtr ImageFile::load(const std::string &filename, const std::string &type, bool srgb)
	{
		SetupDisplay::start();
		auto &types = *SetupDisplay::get_image_provider_factory_types();
		if (type != "")
		{
			if (types.find(type) == types.end()) throw Exception("Unknown image provider type " + type);

			ImageFileType *factory = types[type];
			return factory->load(filename, srgb);
		}

		// Determine file extension and use it to lookup type.
		std::string ext = PathHelp::get_extension(filename, PathHelp::path_type_virtual);
		ext = Text::to_lower(ext);
		if (types.find(ext) == types.end()) throw Exception(std::string("Unknown image provider type ") + ext);

		ImageFileType *factory = types[ext];
		return factory->load(filename, srgb);
	}

	PixelBufferPtr ImageFile::load(IODevice &file, const std::string &type, bool srgb)
	{
		SetupDisplay::start();
		auto &types = *SetupDisplay::get_image_provider_factory_types();
		if (types.find(type) == types.end()) throw Exception("Unknown image provider type " + type);

		ImageFileType *factory = types[type];
		return factory->load(file, srgb);
	}

	void ImageFile::save(PixelBufferPtr buffer, const std::string &filename, const std::string &type)
	{
		SetupDisplay::start();
		auto file = File::open_existing(filename);
		return ImageFile::save(buffer, *file, type);
	}

	void ImageFile::save(PixelBufferPtr buffer, IODevice &file, const std::string &type)
	{
		SetupDisplay::start();

		auto &types = *SetupDisplay::get_image_provider_factory_types();
		if (types.find(type) == types.end()) throw Exception("Unknown image provider type " + type);

		ImageFileType *factory = types[type];
		factory->save(buffer, file);
	}
}
