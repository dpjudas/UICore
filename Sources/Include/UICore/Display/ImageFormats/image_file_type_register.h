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

#include "image_file_type.h"

namespace uicore
{
	/// \brief Class template to register a provider type.
	///
	template<class ProviderClass>
	class ImageFileType_Register : public ImageFileType
	{
	public:
		ImageFileType_Register(const std::string &type) : ImageFileType(type)
		{
		}

		virtual PixelBufferPtr load(const std::string &filename, bool srgb) override
		{
			return ProviderClass::load(filename, srgb);
		}

		virtual PixelBufferPtr load(const IODevicePtr &file, bool srgb) override
		{
			return ProviderClass::load(file, srgb);
		}

		virtual void save(PixelBufferPtr buffer, const std::string &filename) override
		{
			ProviderClass::save(buffer, filename);
		}

		virtual void save(PixelBufferPtr buffer, const IODevicePtr &file) override
		{
			ProviderClass::save(buffer, file);
		}
	};
}
