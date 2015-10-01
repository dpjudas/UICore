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
**    Mark Page
*/


#pragma once

#include <memory>

namespace uicore
{
	class DataBuffer;

	/// \brief Random class
	///
	/// Uses the operating system cryptographically secure pseudorandom number generator
	class Random
	{
	public:
		/// \brief Constructs the object
		///
		/// cache_size = Random number cache size
		static std::shared_ptr<Random> create(int cache_size = 1024);

		/// \brief Get the random bytes
		///
		/// out_dest_ptr = Where to write to
		/// num_bytes = Number of bytes to copy
		virtual void get_random_bytes(unsigned char *out_dest_ptr, int num_bytes) = 0;

		/// \brief Get the random bytes excluding bytes containing 0
		///
		/// out_dest_ptr = Where to write to
		/// num_bytes = Number of bytes to copy
		virtual void get_random_bytes_nzero(unsigned char *out_dest_ptr, int num_bytes) = 0;

		/// \brief Get the random boolean
		///
		/// out_dest_ptr = Where to write to
		/// num_bytes = Number of bytes to copy
		virtual bool get_random_bool() = 0;
	};

	typedef std::shared_ptr<Random> RandomPtr;
}
