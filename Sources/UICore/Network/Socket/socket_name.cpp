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
#include "UICore/Network/Socket/socket_name.h"
#include "UICore/Core/Text/text.h"
#include "UICore/Core/Text/string_format.h"
#ifdef WIN32
typedef unsigned long in_addr_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#ifndef WIN32
#include "stdlib.h"
#include "string.h"
#endif

namespace uicore
{
	bool SocketName::operator == (const SocketName &other_instance) const
	{
		if (StringHelp::compare(other_instance._address, _address, true) != 0)
			return false;
		if (StringHelp::compare(other_instance._port, _port, true) != 0)
			return false;
		return true;
	}

	bool SocketName::operator < (const SocketName &other_instance) const
	{
		int difference = StringHelp::compare(other_instance._address, _address, true);
		if (difference == 0)
			difference = StringHelp::compare(other_instance._port, _port, true);
		return difference < 0;
	}

	bool SocketName::operator >(const SocketName &other_instance) const
	{
		int difference = StringHelp::compare(other_instance._address, _address, true);
		if (difference == 0)
			difference = StringHelp::compare(other_instance._port, _port, true);
		return difference > 0;
	}

	std::string SocketName::lookup_ipv4() const
	{
		in_addr_t ipv4_address = inet_addr(StringHelp::text_to_local8(_address).c_str());
		if (ipv4_address == INADDR_NONE)
		{
			hostent *host = gethostbyname(StringHelp::text_to_local8(_address).c_str());
			if (host == nullptr)
				throw Exception("Could not lookup DNS name");

			ipv4_address = *((in_addr_t *)host->h_addr_list[0]);
		}

		unsigned long addr_long = (unsigned long)ntohl(ipv4_address);
		return string_format(
			"%1.%2.%3.%4",
			int((addr_long & 0xff000000) >> 24),
			int((addr_long & 0x00ff0000) >> 16),
			int((addr_long & 0x0000ff00) >> 8),
			int((addr_long & 0x000000ff)));
	}

	std::string SocketName::lookup_hostname() const
	{
		in_addr_t ipv4_address = inet_addr(StringHelp::text_to_local8(_address).c_str());
		if (ipv4_address != INADDR_NONE)
		{
			in_addr addr;
			memset(&addr, 0, sizeof(in_addr));
			addr.s_addr = ipv4_address;
			hostent *host = gethostbyaddr((const char *)&addr, sizeof(in_addr), AF_INET);
			if (host == nullptr)
				throw Exception("Could not lookup DNS name");

			return StringHelp::local8_to_text(host->h_name);
		}

		return _address;
	}

	SocketName SocketName::to_ipv4()
	{
		return SocketName(lookup_ipv4(), _port);
	}

	SocketName SocketName::to_hostname()
	{
		return SocketName(lookup_hostname(), _port);
	}

	void SocketName::to_sockaddr(int domain, struct sockaddr *out_addr, int len) const
	{
		if (domain != AF_INET)
			throw Exception("Only AF_INET domain supported for SocketName::to_sockaddr");

		if (len < sizeof(sockaddr_in))
			throw Exception("Insufficient buffer for sockaddr_in structure");

		sockaddr_in addr;
		memset(&addr, 0, sizeof(sockaddr_in));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(StringHelp::text_to_int(_port));
		if (_address.empty())
		{
			addr.sin_addr.s_addr = INADDR_ANY;
		}
		else
		{
			addr.sin_addr.s_addr = inet_addr(StringHelp::text_to_local8(_address).c_str());
			if (addr.sin_addr.s_addr == INADDR_NONE)
			{
				hostent *host = gethostbyname(StringHelp::text_to_local8(_address).c_str());
				if (host == nullptr)
					throw Exception("Could not lookup DNS name");

				addr.sin_addr.s_addr = *((in_addr_t *)host->h_addr_list[0]);
			}
		}

		memcpy(out_addr, &addr, sizeof(sockaddr_in));
	}

	void SocketName::from_sockaddr(int domain, struct sockaddr *addr, int len)
	{
		if (domain != AF_INET)
			throw Exception("Only AF_INET domain supported for SocketName::to_sockaddr");

		if (len < sizeof(sockaddr_in))
			throw Exception("Insufficient buffer for sockaddr_in structure");

		sockaddr_in *addr_in = (sockaddr_in *)addr;
		if (addr_in->sin_family != AF_INET)
			throw Exception("Unexpected sin_family in sockaddr");

		unsigned long addr_long = (unsigned long)ntohl(addr_in->sin_addr.s_addr);
		std::string port = StringHelp::int_to_text(ntohs(addr_in->sin_port));

		if (addr_in->sin_addr.s_addr == INADDR_ANY)
		{
			_address = std::string();
		}
		else
		{
			std::string str_addr = string_format(
				"%1.%2.%3.%4",
				int((addr_long & 0xff000000) >> 24),
				int((addr_long & 0x00ff0000) >> 16),
				int((addr_long & 0x0000ff00) >> 8),
				int((addr_long & 0x000000ff)));

			_address = str_addr;
		}

		_port = port;
	}
}
