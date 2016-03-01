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

#include <memory>

namespace uicore
{
	/// \brief Directory scanning class.
	///
	///  DirectoryScanner is used to parse through directory trees and return information about files.
	///  
	///  Example that prints all files and directories found in the root directory:
	///
	///  DirectoryScanner scanner;
	///  if (scanner.scan("/", "*"))
	///  {
	///      while (scanner.next())
	///      {
	///          auto name = scanner.name();
	///      }
	///  }
	///
	class DirectoryScanner
	{
	public:
		/// \brief Constructs directory scanner for iterating over a directory.
		static std::shared_ptr<DirectoryScanner> create();

		/// \brief Gets the directory being scanned.
		/** \return Directory being scanned. (including the trailing slash)*/
		virtual std::string directory_path() = 0;

		/// \brief Gets the name of the current file.
		/** \return The name of the current found file.*/
		virtual std::string name() = 0;

		/// \brief Gets the size of the current file.
		/** \return The size of the current found file.*/
		virtual int size() = 0;

		/// \brief Gets the pathname of the current file.
		/** \return The name of the current found file, including the directory path.*/
		virtual std::string pathname() = 0;

		/// \brief Returns true if the current file is a directory.
		/** \return True if filename is a directory.*/
		virtual bool is_directory() = 0;

		/// \brief Returns true if the file is hidden.
		/** \return True if filename is hidden.*/
		virtual bool is_hidden() = 0;

		/// \brief Returns true if the file is readable by the current user.
		/** \return True if the file is readable.*/
		virtual bool is_readable() = 0;

		/// \brief Returns true if the file is writable by the current user.
		/** \return True if the file is writable.*/
		virtual bool is_writable() = 0;

		/// \brief Selects the directory to scan through.
		/** Selects the directory to scan through
			\param pathname Path to the directory to scan (without trailing slash)
			\return true if the directory can be accessed.*/
		virtual bool scan(const std::string& pathname) = 0;

		/// \brief Selects the directory to scan through.
		/** Selects the directory to scan through and use a matching pattern on the files.
			WIN32: The pattern is normal DOS pattern matching ("*.*", ?)
			Unix: The pattern is normal pattern matching (*, ?)
			\param pathname Path to the directory to scan (without trailing slash)
			\param pattern Pattern to match files against.
			\return true if the directory can be accessed.*/
		virtual bool scan(const std::string& pathname, const std::string& pattern) = 0;

		/// \brief Find next file in directory scan.
		/** \return false if no more files was found.*/
		virtual bool next() = 0;
	};
}
