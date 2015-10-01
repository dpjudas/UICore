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

#include <map>

namespace uicore
{
	class IODevice;
	class PixelBuffer;
	class ImageFileType;

	/// \brief Load or save an image
	class ImageFile
	{
	public:
		static PixelBuffer try_load(const std::string &filename, const std::string &type = std::string(), std::string *out_failure_reason = nullptr, bool srgb = false);

		static PixelBuffer load(const std::string &filename, const std::string &type = std::string(), bool srgb = false);
		static PixelBuffer load(IODevice &file, const std::string &type, bool srgb = false);

		static void save(PixelBuffer buffer, const std::string &filename, const std::string &type = std::string());
		static void save( PixelBuffer buffer, IODevice &file, const std::string &type);
	};
}
