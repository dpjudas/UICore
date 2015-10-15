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

#include <string>
#include <memory>
#include "iodevice.h"
#include "../System/databuffer.h"

namespace uicore
{
	enum class FileAccess
	{
		read,
		write,
		read_write,
	};

	class File : public IODevice
	{
	public:
		static std::shared_ptr<File> open_existing(const std::string &filename, FileAccess access = FileAccess::read);
		static std::shared_ptr<File> open_always(const std::string &filename, FileAccess access = FileAccess::read_write);
		static std::shared_ptr<File> create_always(const std::string &filename, FileAccess access = FileAccess::read_write);
		static std::shared_ptr<File> create_new(const std::string &filename, FileAccess access = FileAccess::read_write);

		static std::string read_all_text(const std::string &filename);
		static void write_all_text(const std::string &filename, const std::string &text);

		static DataBufferPtr read_all_bytes(const std::string &filename);
		static void write_all_bytes(const std::string &filename, const DataBufferPtr &data);

		static void copy(const std::string &from, const std::string &to, bool copy_always);
		static void remove(const std::string &filename);
		static bool exists(const std::string &filename);
	};

	typedef std::shared_ptr<File> FilePtr;
}
