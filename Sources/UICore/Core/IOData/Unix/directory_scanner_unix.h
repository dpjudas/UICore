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

#include "UICore/Core/IOData/directory_scanner.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>

namespace uicore
{
	class DirectoryScanner_Unix : public DirectoryScanner
	{
	public:
		DirectoryScanner_Unix();
		~DirectoryScanner_Unix();

		bool scan(const std::string& pathname) override;
		bool scan(const std::string& pathname, const std::string& pattern) override;
		std::string get_directory_path() override;
		int get_size() override;
		std::string get_name() override;
		std::string get_pathname() override;
		bool is_directory() override;
		bool is_hidden() override;
		bool is_readable() override;
		bool is_writable() override;
		bool next() override;

	private:
		DIR *dir_temp;
		dirent *entry;
		struct stat statbuf;
		bool use_pattern;
		/// \brief Path stored without the trailing slash
		std::string file_name;
		std::string path_name;
		std::string file_pattern;
	};
}
