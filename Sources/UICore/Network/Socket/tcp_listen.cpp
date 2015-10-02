
#include "UICore/precomp.h"
#include "UICore/Network/Socket/tcp_listen.h"
#include "UICore/Network/Socket/tcp_connection.h"
#include "UICore/Network/Socket/socket_name.h"
#include "UICore/Core/System/exception.h"
#include "tcp_socket.h"

namespace uicore
{

#if defined(WIN32)

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

	TCPListenImpl::TCPListenImpl(const SocketName &endpoint, int backlog, bool reuse_address)
	{
		if (reuse_address)
		{
			int value = 1;
			int result = setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, (const char *)&value, sizeof(int));
			if (result == SOCKET_ERROR)
				throw Exception("Could not set reuse address socket option");
		}

		int receive_buffer_size = 600 * 1024;
		int send_buffer_size = 600 * 1024;

		//int result = setsockopt(handle, SOL_SOCKET, SO_RCVBUF, (const char *) &receive_buffer_size, sizeof(int));
		//if (result == SOCKET_ERROR)
		//	throw Exception("Could not set socket receive buffer size");

		int result = setsockopt(handle, SOL_SOCKET, SO_SNDBUF, (const char *)&send_buffer_size, sizeof(int));
		if (result == SOCKET_ERROR)
			throw Exception("Could not set socket send buffer size");

		int value = 1;
		result = setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, (const char *)&value, sizeof(int));

		sockaddr_in addr;
		endpoint.to_sockaddr(AF_INET, (sockaddr *)&addr, sizeof(sockaddr_in));
		result = ::bind(handle, (const sockaddr *)&addr, sizeof(sockaddr_in));
		if (result == SOCKET_ERROR)
			throw Exception("Could not bind socket to end point");

		result = ::listen(handle, backlog);
		if (result == SOCKET_ERROR)
			throw Exception("Unable to set socket into listen state");

		result = WSAEventSelect(handle, wait_handle, FD_ACCEPT);
		if (result == SOCKET_ERROR)
			throw Exception("WSAEventSelect failed");
	}

	std::shared_ptr<TCPConnection> TCPListenImpl::accept(SocketName &out_end_point)
	{
		sockaddr_in peer_address = { 0 };
		int peer_address_length = sizeof(sockaddr_in);

		SOCKET result = ::accept(handle, reinterpret_cast<sockaddr*>(&peer_address), &peer_address_length);
		if (result == INVALID_SOCKET)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				return std::shared_ptr<TCPConnection>();
			else
				throw Exception("Error accepting from socket");
		}

		out_end_point = SocketName();
		out_end_point.from_sockaddr(AF_INET, reinterpret_cast<sockaddr*>(&peer_address), peer_address_length);
		return std::make_shared<TCPConnectionImpl>(result);
	}

#else

	TCPListenImpl::TCPListenImpl(const SocketName &endpoint, int backlog, bool reuse_address)
	{
		if (reuse_address)
		{
			int value = 1;
			int result = setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, (const char *) &value, sizeof(int));
			if (result == -1)
				throw Exception("Could not set reuse address socket option");
		}

		//int receive_buffer_size = 600*1024;
		int send_buffer_size = 600*1024;

		// int result = setsockopt(handle, SOL_SOCKET, SO_RCVBUF, (const char *) &receive_buffer_size, sizeof(int));
		// if (result == -1)
		//	throw Exception("Could not set socket receive buffer size");

		int result = setsockopt(handle, SOL_SOCKET, SO_SNDBUF, (const char *) &send_buffer_size, sizeof(int));
		if (result == -1)
			throw Exception("Could not set socket send buffer size");

		int value = 1;
		result = setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, (const char *) &value, sizeof(int));

		sockaddr_in addr;
		endpoint.to_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
		result = ::bind(handle, (const sockaddr *) &addr, sizeof(sockaddr_in));
		if (result == -1)
			throw Exception("Could not bind socket to end point");

		result = ::listen(handle, backlog);
		if (result == -1)
			throw Exception("Unable to set socket into listen state");

		int nonblocking = 1;
		ioctl(handle, FIONBIO, &nonblocking);
	}

	std::shared_ptr<TCPConnection> TCPListenImpl::accept(SocketName &out_end_point)
	{
		sockaddr_in peer_address = { 0 };
		socklen_t peer_address_length = sizeof(sockaddr_in);

		int result = ::accept(handle, reinterpret_cast<sockaddr*>(&peer_address), &peer_address_length);
		if (result == -1)
		{
			if (errno == EWOULDBLOCK)
				return std::shared_ptr<TCPConnection>();
			else
				throw Exception("Error accepting from socket");
		}

		out_end_point.from_sockaddr(AF_INET, reinterpret_cast<sockaddr*>(&peer_address), peer_address_length);
		return std::make_shared<TCPConnectionImpl>(result);
	}

#endif

	std::shared_ptr<TCPListen> TCPListen::listen(const SocketName &endpoint, int backlog, bool reuse_address)
	{
		return std::make_shared<TCPListenImpl>(endpoint, backlog, reuse_address);
	}
}
