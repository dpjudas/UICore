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


#pragma once

#include <memory>

struct sockaddr;

namespace uicore
{
	/// \brief Socket name; container class for an IP address and port.
	class SocketName
	{
	public:
		/// \brief Constructs a new socket name.
		///
		/// If no address is specified, the socket address will default to INADDR_ANY. 
		/// If no port is specified, it will default to port 0.
		SocketName() : _port("0") { }
		SocketName(const std::string &port) : _port(port) { }
		SocketName(const std::string &address, const std::string &port) : _address(address), _port(port) { }

		/// \brief Returns the address part of the socket name.
		const std::string &address() const { return _address; }

		/// \brief Returns the port part of the socket name.
		const std::string &port() const { return _port; }

		/// \brief Set the socket name using a hostname and port.
		///
		/// \param hostname Can be an IPv4 dotted-quad, hostname or a valid IPv6 address.
		/// \param port Port number.
		void set_name(const std::string &hostname, const std::string &port) { _address = hostname; _port = port; }

		/// \brief Set the IP address.
		/// \param address Can be an IPv4 dotted-quad, hostname or a valid IPv6 address.
		void set_address(const std::string &address) { _address = address; }

		/// \brief Set the IP port.
		void set_port(const std::string &port) { _port = port; }

		/// \brief Perform a DNS lookup, if needed, for the IP v4 address.
		std::string lookup_ipv4() const;

		/// \brief Perform a DNS lookup, if needed, for the hostname.
		std::string lookup_hostname() const;

		/// \brief Create socket name that uses the IP v4 address as its address.
		SocketName to_ipv4();

		/// \brief Create socket name that uses the hostname as its address.
		SocketName to_hostname();

		/// \brief Fill the socket name into a C sockets sockaddr structure.
		void to_sockaddr(int domain, sockaddr *addr, int len) const;

		/// \brief Get the socket name from a C sockets sockaddr structure.
		void from_sockaddr(int domain, sockaddr *addr, int len);

		bool operator == (const SocketName &other_instance) const;
		bool operator < (const SocketName &other_instance) const;

	private:
		std::string _address;
		std::string _port;
	};
}
