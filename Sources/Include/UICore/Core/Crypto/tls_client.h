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
**    Mark Page
**    Magnus Norddahl
*/

#pragma once

#include <memory>

namespace uicore
{
	/// \brief Transport Layer Security (TLS) client class
	class TLSClient
	{
	public:
		/// \brief Constructs a TLS client
		static std::shared_ptr<TLSClient> create();

		/// \brief Return a pointer to decrypted data available for consumption.
		virtual const void *decrypted_data() const = 0;

		/// \brief Returns how much decrypted data is available.
		virtual int decrypted_data_available() const = 0;

		/// \brief Return a pointer to encrypted data available for consumption.
		virtual const void *encrypted_data() const = 0;

		/// \brief Returns how much encrypted data is available.
		virtual int encrypted_data_available() const = 0;

		/// \brief Adds data to be encrypted.
		virtual int encrypt(const void *data, int size) = 0;

		/// \brief Adds data to be decrypted.
		virtual int decrypt(const void *data, int size) = 0;

		/// \brief Marks decrypted data as consumed.
		virtual void decrypted_data_consumed(int size) = 0;

		/// \brief Marks encrypted data as consumed.
		virtual void encrypted_data_consumed(int size) = 0;
	};

	typedef std::shared_ptr<TLSClient> TLSClientPtr;
}
