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

#include <vector>

namespace uicore
{
	/// \brief File path types.
	enum class FilePathType
	{
		/// \brief Native file system path. (On windows, '/' are turned into '\'. On linux, '\' are turned into '/')
		file_system,

		/// \brief Path using slashes ('/').
		slashes
	};

	/// \brief Path helper functions.
	class FilePath
	{
	public:
		/// Changes a path to be relative to the specified base path, if possible.
		static std::string make_absolute(const std::string &base_path, const std::string &relative_path, FilePathType path_type = FilePathType::file_system);

		/// Changes a path to be relative to the specified base path, if possible.
		static std::string make_relative(const std::string &base_path, const std::string &absolute_path, FilePathType path_type = FilePathType::file_system);

		/// Returns the absolute file system path for the given path
		static std::string absolute_path(std::string path);

		/// \brief Returns the drive (C:) or share name ( \ \ computer \ share)
		static std::string location(const std::string &path);

		/// \brief Check if a path is absolute.
		///
		/// \param path The path to check
		/// \param path_type The path type (of path)
		/// \return true if path is absolute
		static bool is_absolute(const std::string &path, FilePathType path_type = FilePathType::file_system);

		/// \brief Check if a path is relative.
		///
		/// \param path The path to check
		/// \param path_type The path type (of path)
		/// \return true if path is relative
		static bool is_relative(const std::string &path, FilePathType path_type = FilePathType::file_system);

		/// \brief  Normalize a path.
		///
		/// \param path The path to use
		/// \param path_type The path type (of the normalized path)
		/// \return The converted path
		///
		/// Converts all slashes and backslashes into their right type.
		/// Simplifies or removes all . and .., converting the path into its
		/// most readable form.
		static std::string normalize(const std::string &path, FilePathType path_type = FilePathType::file_system);

		/// \brief  Add trailing slash or backslash to path.
		///
		/// This function checks if a path already has a trailing
		/// slash/backslash and adds it if its missing.
		/// \param path The path to use
		/// \param path_type The path type (of path)
		/// \return The converted path
		static std::string add_trailing_slash(const std::string &path, FilePathType path_type = FilePathType::file_system);

		/// \brief  Remove trailing slash or backslash from path.
		///
		/// This function checks if a path has a trailing
		/// slash/backslash and removes it if needed.
		/// \param path The path to use
		/// \return The converted path
		static std::string remove_trailing_slash(const std::string &path);

		/// Returns a new path with the last path component removed
		static std::string basepath(const std::string &path, FilePathType path_type = FilePathType::file_system);

		/// Returns the filename part of a path
		static std::string filename(const std::string &path, FilePathType path_type = FilePathType::file_system);

		/// Returns the extension part (includes the dot) of a path
		static std::string extension(const std::string &path, FilePathType path_type = FilePathType::file_system);

		/// Returns the filename part without the extension
		static std::string filename_without_extension(const std::string &path, FilePathType path_type = FilePathType::file_system);

		/// Checks if the path has the specified extension
		static bool has_extension(const std::string &path, const std::string &extension, FilePathType path_type = FilePathType::file_system);

		/// \brief Concatenates two strings into one adding a trailing slash to first string if missing.
		/// \param part1 First part of path
		/// \param part2 Second part of path
		/// \param path_type The path type (of path)
		static std::string combine(const std::string &part1, const std::string &part2, FilePathType path_type = FilePathType::file_system);
	};
}
