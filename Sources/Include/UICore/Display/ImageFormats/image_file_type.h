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
*/

#pragma once

#include "../Image/pixel_buffer.h"

namespace uicore
{
	class IODevice;
	typedef std::shared_ptr<IODevice> IODevicePtr;

	/// \brief Provider type
	class ImageFileType
	{
	public:
		/// \brief Registers a provider type in the ImageFile.
		ImageFileType(const std::string &type);

		/// \brief Unregisters a provider type in the ImageFile.
		virtual ~ImageFileType();

		/// \brief Called to load an image with this provider type.
		virtual PixelBufferPtr load(const std::string &filename, bool srgb) = 0;
		virtual PixelBufferPtr load(const IODevicePtr &file, bool srgb) = 0;

		/// \brief Called to save a given PixelBuffer to a file
		virtual void save(PixelBufferPtr buffer, const std::string &filename) = 0;
		virtual void save(PixelBufferPtr buffer, const IODevicePtr &file) = 0;
	};
}
