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

#include "UICore/precomp.h"
#include "UICore/Core/System/exception.h"
#include "UICore/Core/IOData/file_system.h"
#include "UICore/Core/IOData/path_help.h"
#include "UICore/Display/ImageFormats/targa_format.h"
#include "UICore/Display/Image/pixel_buffer.h"
#include "UICore/Display/ImageFormats/TargaLoader/targa_loader.h"

namespace uicore
{
	PixelBuffer TargaFormat::load(
		const std::string &filename,
		const FileSystem &fs,
		bool srgb)
	{
		IODevice datafile = fs.open_file(filename);
		return TargaLoader::load(datafile, srgb);
	}

	PixelBuffer TargaFormat::load(
		const std::string &fullname,
		bool srgb)
	{
		File file(fullname);
		return TargaLoader::load(file, srgb);
	}

	PixelBuffer TargaFormat::load(
		IODevice &file,
		bool srgb)
	{
		return TargaLoader::load(file, srgb);
	}

	void TargaFormat::save(
		PixelBuffer buffer,
		const std::string &filename,
		FileSystem &fs)
	{
		throw Exception("TargaFormat doesn't support saving");
	}

	void TargaFormat::save(
		PixelBuffer buffer,
		IODevice &file)
	{
		throw Exception("TargaFormat doesn't support saving");
	}

	void TargaFormat::save(
		PixelBuffer buffer,
		const std::string &fullname)
	{
		std::string path = PathHelp::get_fullpath(fullname, PathHelp::path_type_file);
		std::string filename = PathHelp::get_filename(fullname, PathHelp::path_type_file);
		FileSystem vfs(path);
		TargaFormat::save(buffer, filename, vfs);
	}
}
