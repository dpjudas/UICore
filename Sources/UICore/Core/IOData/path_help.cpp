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
**    Kenneth Gangstoe
*/

#include "UICore/precomp.h"
#include "UICore/Core/IOData/path_help.h"
#include "UICore/Core/System/exception.h"
#include "UICore/Core/Text/string_format.h"
#include "UICore/Core/Text/text.h"
#ifdef WIN32
#if defined UNICODE && !defined _UNICODE
#define _UNICODE
#endif
#include <tchar.h>
#include <direct.h>
#include <wchar.h>
#endif

#ifndef WIN32
#include <cstring>
#endif

namespace uicore
{
	std::string FilePath::absolute_path(std::string path)
	{
		path = normalize(path, FilePathType::file_system);
		if (is_absolute(path, FilePathType::file_system))
			return path;

#ifdef WIN32
		WCHAR absolute_path[MAX_PATH];
		memset(absolute_path, 0, sizeof(WCHAR) * MAX_PATH);
		if (_wfullpath(absolute_path, Text::to_utf16(path).c_str(), MAX_PATH) == 0)
			throw Exception(string_format("Unable to make base path absolute: %1", path));
		path = Text::from_utf16(absolute_path);
#else
		char working_dir[1024];
		memset(working_dir, 0, 1024);
		if (getcwd(working_dir, 1024) == nullptr)
			throw Exception("Unable to get current working directory!");
		path = combine(working_dir, path, FilePathType::file_system);
#endif

		return path;
	}

	std::string FilePath::location(const std::string &path)
	{
#ifdef WIN32
		if (path.length() >= 2)
		{
			if (path[1] == ':')
			{
				return path.substr(0, 2);
			}
			else if (path[0] == '\\' && path[1] == '\\')
			{
				std::string::size_type pos = path.find_first_of("/\\", 2);
				if (pos == std::string::npos)
					return path;
				else
					return path.substr(0, pos);
			}
		}
#endif
		return std::string();
	}

	std::string FilePath::make_absolute(const std::string &base_path, const std::string &relative_path, FilePathType path_type)
	{
		std::string path = normalize(combine(base_path, relative_path));
		if (path_type == FilePathType::file_system)
			path = absolute_path(path);
		return path;
	}

	std::string FilePath::make_relative(const std::string &base_path, const std::string &absolute_path, FilePathType path_type)
	{
		if (is_relative(base_path, path_type))
			throw Exception(string_format("Relative path %1 used as base path for make_relative", base_path));

		if (is_relative(absolute_path))
			return normalize(combine(base_path, absolute_path), path_type);

		std::string base = add_trailing_slash(normalize(base_path, path_type), path_type);
		std::string absolute = normalize(absolute_path, path_type);

		std::string location_base = location(base);
		std::string location_absolute = location(absolute);
		if (!Text::equal_caseless(location_base, location_absolute))
			return absolute_path;

		std::string relative;
		std::string relative_end;

		bool differs = false;
		std::string::size_type start_pos = 0, end_pos = 0;

		while (true)
		{
			if (path_type == FilePathType::file_system)
			{
				end_pos = base.find_first_of("\\/", start_pos);
			}
			else
			{
				end_pos = base.find('/', start_pos);
			}
			if (end_pos == std::string::npos)
				break;

			if (!differs)
			{
				std::string base_element = base.substr(start_pos, end_pos - start_pos + 1);
				std::string absolute_element = absolute.substr(start_pos, end_pos - start_pos + 1);

				bool same_element = false;
				if (path_type == FilePathType::file_system)
				{
#ifdef WIN32
					same_element = Text::equal_caseless(base_element, absolute_element);
#else
					same_element = (base_element == absolute_element);
#endif
				}
				else
				{
					same_element = (base_element == absolute_element);
				}

				if (!same_element)
				{
					relative_end = absolute.substr(start_pos);
					differs = true;
				}
				else
				{
					relative_end = absolute.substr(end_pos + 1);
				}
			}

			if (differs)
			{
				if (path_type == FilePathType::file_system)
				{
#ifdef WIN32
					relative += "..\\";
#else
					relative += "../";
#endif
				}
				else
				{
					relative += "../";
				}
			}

			start_pos = end_pos + 1;
		}

		return relative + relative_end;
	}

	bool FilePath::is_absolute(const std::string &path, FilePathType path_type)
	{
		if (path_type == FilePathType::file_system)
		{
#ifdef WIN32
			if (path.length() < 3)
				return false;
			if (path[1] == ':' && (path[2] == '\\' || path[2] == '/'))
				return true;
			if (path[0] == '\\' && path[1] == '\\')
				return true;
			return false;
#else
			if (path.length() < 1)
				return false;
			if (path[0] == '/')
				return true;
			if (path[0] == '\\')
				return true;
			return false;
#endif
		}
		else
		{
			if (path.length() < 1)
				return false;
			if (path[0] == '/')
				return true;
			return false;
		}
	}

	bool FilePath::is_relative(const std::string &path, FilePathType path_type)
	{
		return !is_absolute(path, path_type);
	}

	std::string FilePath::normalize(
		const std::string &input_path,
		FilePathType path_type)
	{
		if (input_path.empty())
			return std::string();

		std::string path_location;
		if (path_type == FilePathType::file_system)
			path_location = location(input_path);
		std::string path = input_path.substr(path_location.length());

		bool ends_in_slash = false;

		if (input_path[input_path.size() - 1] == '/' ||
			input_path[input_path.size() - 1] == '\\')
		{
			ends_in_slash = true;
			if (input_path.size() == 1)
			{
#ifdef WIN32
				if (path_type == FilePathType::file_system)
				{
					return "\\";
				}
#endif
				return "/";
			}
		}

		std::vector<std::string> elements;
		bool absolute = false;
		int level = 0;
		std::string::size_type pos = 0, last_pos = 0;

		while (true)
		{
			pos = path.find_first_of("/\\", last_pos);

			if (pos == std::string::npos)
				pos = path.length();

			if (pos == 0)
			{
				absolute = true;
			}
			else
			{
				std::string element = path.substr(last_pos, pos - last_pos);
				if (element.empty() && pos != path.length())
					throw Exception(string_format("Unable to normalize invalid path %1", input_path));

				if (element.empty())
				{
				}
				else if (element == ".")
				{
				}
				else if (element == "..")
				{
					level--;
					if (!elements.empty())
					{
						if (elements[elements.size() - 1] != "..")
							elements.erase(elements.begin() + elements.size() - 1);
						else
							elements.push_back("..");
					}
					else
					{
						elements.push_back("..");
					}

					if (absolute && level < 0)
						throw Exception(string_format("Unable to normalize invalid path %1", input_path));
				}
				else
				{
					elements.push_back(element);
					level++;
				}
			}

			if (pos == path.length())
				break;

			last_pos = pos + 1;
		}

		std::string normalized_path;

		if (absolute)
		{
#ifdef WIN32
			if (path_type == FilePathType::file_system)
				normalized_path += "\\";
			else
				normalized_path += "/";
#else
			normalized_path += "/";
#endif
		}

		for (auto & element : elements)
		{
#ifdef WIN32
			if (path_type == FilePathType::file_system)
				normalized_path += element + "\\";
			else
				normalized_path += element + "/";
#else
			normalized_path += element + "/";
#endif
		}
		if (!elements.empty() && !ends_in_slash)
			normalized_path = normalized_path.substr(0, normalized_path.length() - 1);

		return path_location + normalized_path;
	}

	std::string FilePath::add_trailing_slash(const std::string &path, FilePathType path_type)
	{
		if (path.empty())
			return path;
		if (path_type == FilePathType::file_system)
		{
#ifdef WIN32
			if (path[path.length() - 1] != '/' && path[path.length() - 1] != '\\')
				return path + "\\";
#else
			if (path[path.length()-1] != '/' && path[path.length()-1] != '\\')
				return path + "/";
#endif
			return path;
		}
		else
		{
			if (path[path.length() - 1] != '/')
				return path + "/";
			return path;
		}
	}

	std::string FilePath::remove_trailing_slash(const std::string &path)
	{
		if (path.empty())
			return path;

		if (path[path.length() - 1] == '/' || path[path.length() - 1] == '\\')
			return path.substr(0, path.length() - 1);

		return path;
	}

	std::string FilePath::basepath(const std::string &path, FilePathType path_type)
	{
#ifdef WIN32
		if (path_type == FilePathType::file_system)
		{
			auto last_slash = path.find_last_of("/\\");
			if (last_slash != std::string::npos)
				return path.substr(0, last_slash + 1);
			else
				return std::string();
		}
#endif

		auto last_slash = path.find_last_of('/');
		if (last_slash != std::string::npos)
			return path.substr(0, last_slash + 1);
		else
			return std::string();
	}

	std::string FilePath::filename(const std::string &path, FilePathType path_type)
	{
#ifdef WIN32
		if (path_type == FilePathType::file_system)
		{
			auto last_slash = path.find_last_of("/\\");
			if (last_slash != std::string::npos)
				return path.substr(last_slash + 1);
			else
				return path;
		}
#endif

		auto last_slash = path.find_last_of('/');
		if (last_slash != std::string::npos)
			return path.substr(last_slash + 1);
		else
			return path;
	}

	std::string FilePath::filename_without_extension(const std::string &path, FilePathType path_type)
	{
		std::string file = filename(path, path_type);
		std::string::size_type pos = file.find_last_of('.');
		if (pos == std::string::npos)
			return file;
#ifndef WIN32
		// Files beginning with a dot is not a filename extension in Unix.
		// This is different from Windows where it is considered the extension.
		if (path_type == FilePathType::file_system && pos == 0)
			return file;
#endif
		if (path_type == FilePathType::slashes && pos == 0)
			return file;
		return file.substr(0, pos);
	}

	std::string FilePath::extension(const std::string &path, FilePathType path_type)
	{
		std::string file = filename(path, path_type);
		std::string::size_type pos = file.find_last_of('.');
		if (pos == std::string::npos)
			return std::string();
#ifndef WIN32
		// Files beginning with a dot is not a filename extension in Unix.
		// This is different from Windows where it is considered the extension.
		if (path_type == FilePathType::file_system && pos == 0)
			return std::string();
#endif
		if (path_type == FilePathType::slashes && pos == 0)
			return std::string();
		return file.substr(pos + 1);
	}

	bool FilePath::has_extension(const std::string &path, const std::string &test_extension, FilePathType path_type)
	{
		return Text::equal_caseless(extension(path, path_type), test_extension);
	}

	std::string FilePath::combine(const std::string &part1, const std::string &part2, FilePathType path_type)
	{
		if (!part1.empty())
		{
			// Absolute paths always override the base path
			if (is_absolute(part2, path_type))
				return part2;

			// Drive letters also override the base path
			if (path_type == FilePathType::file_system && !location(part2).empty())
				return part2;

			return add_trailing_slash(part1, path_type) + part2;
		}
		else
		{
			return part2;
		}
	}
}
