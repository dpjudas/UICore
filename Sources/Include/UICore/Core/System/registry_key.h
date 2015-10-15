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

#if defined(WIN32) || defined(DOXYGEN)

#include "databuffer.h"
#include <vector>

namespace uicore
{
	/// \brief Registry key class.
	///
	/// This class is only available on Windows.
	class RegistryKey
	{
	public:
		enum PredefinedKey
		{
			key_classes_root,
			key_current_config,
			key_current_user,
			key_local_machine,
			key_users
		};

		enum CreateFlags
		{
			create_always = 0,
			create_new = 1,
			create_volatile = 2
		};

		static std::shared_ptr<RegistryKey> create(PredefinedKey key, const std::string &subkey, unsigned int access_rights = KEY_ALL_ACCESS, unsigned int create_flags = create_always);
		static std::shared_ptr<RegistryKey> create(HKEY key);

		static void delete_key(PredefinedKey key, const std::string &subkey, bool recursive);

		virtual HKEY get_key() const = 0;

		virtual std::shared_ptr<RegistryKey> open_key(const std::string &subkey, unsigned int access_rights = KEY_ALL_ACCESS) = 0;
		virtual std::shared_ptr<RegistryKey> create_key(const std::string &subkey, unsigned int access_rights = KEY_ALL_ACCESS, CreateFlags create_flags = create_always) = 0;

		virtual std::vector<std::string> get_subkey_names() const = 0;
		virtual std::vector<std::string> get_value_names() const = 0;

		virtual int get_value_int(const std::string &name, int default_value = 0) const = 0;
		virtual DataBufferPtr get_value_binary(const std::string &name, const DataBufferPtr &default_value = DataBufferPtr()) const = 0;
		virtual std::string get_value_string(const std::string &name, const std::string &default_value = std::string()) const = 0;
		virtual std::vector<std::string> get_value_multi_string(const std::string &name, const std::vector<std::string> &default_value = std::vector<std::string>()) const = 0;

		virtual void set_value_int(const std::string &name, int value) = 0;
		virtual void set_value_binary(const std::string &name, const DataBufferPtr &value) = 0;
		virtual void set_value_string(const std::string &name, const std::string &value) = 0;

		virtual void delete_key(const std::string &subkey, bool recursive) = 0;
		virtual void delete_value(const std::string &name) = 0;
	};

	typedef std::shared_ptr<RegistryKey> RegistryKeyPtr;
}

#endif
