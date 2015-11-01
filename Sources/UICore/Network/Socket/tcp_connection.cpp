
#include "UICore/precomp.h"
#include "UICore/Network/Socket/tcp_connection.h"
#include "UICore/Network/Socket/socket_name.h"
#include "tcp_socket.h"
#include "UICore/Core/System/exception.h"

namespace uicore
{
#if defined(WIN32)

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

	TCPConnectionImpl::TCPConnectionImpl(const SocketName &endpoint)
	{
		int receive_buffer_size = 600 * 1024;
		int send_buffer_size = 600 * 1024;
		//int result = setsockopt(handle, SOL_SOCKET, SO_RCVBUF, (const char *) &receive_buffer_size, sizeof(int));
		int result = setsockopt(handle, SOL_SOCKET, SO_SNDBUF, (const char *)&send_buffer_size, sizeof(int));

		sockaddr_in addr;
		endpoint.to_sockaddr(AF_INET, (sockaddr *)&addr, sizeof(sockaddr_in));
		result = ::connect(handle, (const sockaddr *)&addr, sizeof(sockaddr_in));
		if (result == SOCKET_ERROR)
			throw Exception("Connect to server failed");

		int value = 1;
		result = setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, (const char *)&value, sizeof(int));

		result = WSAEventSelect(handle, wait_handle, FD_READ | FD_WRITE | FD_CLOSE);
		if (result == SOCKET_ERROR)
			throw Exception("WSAEventSelect failed");

		//int receive_buffer_size = 0;
		//int send_buffer_size = 0;
		//int len = sizeof(int);
		//getsockopt(handle, SOL_SOCKET, SO_RCVBUF, (char *) &receive_buffer_size, &len);
		//getsockopt(handle, SOL_SOCKET, SO_SNDBUF, (char *) &send_buffer_size, &len);
	}

	TCPConnectionImpl::TCPConnectionImpl(SOCKET init_handle) : TCPSocket(init_handle)
	{
		int receive_buffer_size = 600 * 1024;
		int send_buffer_size = 600 * 1024;
		//int result = setsockopt(handle, SOL_SOCKET, SO_RCVBUF, (const char *) &receive_buffer_size, sizeof(int));
		int result = setsockopt(handle, SOL_SOCKET, SO_SNDBUF, (const char *)&send_buffer_size, sizeof(int));

		result = WSAEventSelect(handle, wait_handle, FD_READ | FD_WRITE | FD_CLOSE);
		if (result == SOCKET_ERROR)
			throw Exception("WSAEventSelect failed");
	}

	int TCPConnectionImpl::write(const void *data, int size)
	{
		int result = ::send(handle, static_cast<const char *>(data), size, 0);
		if (result == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				return -1;
			else
				throw Exception("Error writing to server");
		}
		return result;
	}

	int TCPConnectionImpl::read(void *data, int size)
	{
		int result = ::recv(handle, static_cast<char *>(data), size, 0);
		if (result == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				return -1;
			else
				throw Exception("Error reading from server");
		}
		return result;
	}

	SocketName TCPConnectionImpl::local_name()
	{
		sockaddr_in addr;
		memset(&addr, 0, sizeof(sockaddr_in));
		int size = sizeof(sockaddr_in);
		int result = getsockname(handle, (sockaddr *)&addr, &size);
		if (result == SOCKET_ERROR)
			throw Exception("Error retrieving local socket name");

		SocketName name;
		name.from_sockaddr(AF_INET, (sockaddr *)&addr, sizeof(sockaddr_in));
		return name;
	}

	SocketName TCPConnectionImpl::remote_name()
	{
		sockaddr_in addr;
		memset(&addr, 0, sizeof(sockaddr_in));
		int size = sizeof(sockaddr_in);
		int result = getpeername(handle, (sockaddr *)&addr, &size);
		if (result == SOCKET_ERROR)
			throw Exception("Error retrieving remote socket name");

		SocketName name;
		name.from_sockaddr(AF_INET, (sockaddr *)&addr, sizeof(sockaddr_in));
		return name;
	}

#else

	TCPConnectionImpl::TCPConnectionImpl(const SocketName &endpoint)
	{
		//int receive_buffer_size = 600*1024;
		int send_buffer_size = 600*1024;
		//int result = setsockopt(handle, SOL_SOCKET, SO_RCVBUF, (const char *) &receive_buffer_size, sizeof(int));
		int result = setsockopt(handle, SOL_SOCKET, SO_SNDBUF, (const char *) &send_buffer_size, sizeof(int));

		sockaddr_in addr;
		endpoint.to_sockaddr(AF_INET, (sockaddr *) &addr, sizeof(sockaddr_in));
		result = ::connect(handle, (const sockaddr *) &addr, sizeof(sockaddr_in));
		if (result == -1)
			throw Exception("Connect to server failed");

		int value = 1;
		result = setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, (const char *) &value, sizeof(int));

		//int receive_buffer_size = 0;
		//int send_buffer_size = 0;
		//int len = sizeof(int);
		//getsockopt(handle, SOL_SOCKET, SO_RCVBUF, (char *) &receive_buffer_size, &len);
		//getsockopt(handle, SOL_SOCKET, SO_SNDBUF, (char *) &send_buffer_size, &len);

		int nonblocking = 1;
		ioctl(handle, FIONBIO, &nonblocking);
	}

	TCPConnectionImpl::TCPConnectionImpl(int init_handle) : TCPSocket(init_handle)
	{
		//int receive_buffer_size = 600*1024;
		int send_buffer_size = 600*1024;
		//int result = setsockopt(handle, SOL_SOCKET, SO_RCVBUF, (const char *) &receive_buffer_size, sizeof(int));
		setsockopt(handle, SOL_SOCKET, SO_SNDBUF, (const char *) &send_buffer_size, sizeof(int));

		int nonblocking = 1;
		ioctl(handle, FIONBIO, &nonblocking);
	}

	int TCPConnectionImpl::write(const void *data, int size)
	{
		int result = ::send(handle, static_cast<const char *>(data), size, 0);
		if (result == -1)
		{
			if (errno == EWOULDBLOCK)
			{
				can_write = false;
				return -1;
			}
			else
			{
				throw Exception("Error writing to server");
			}
		}
		return result;
	}

	int TCPConnectionImpl::read(void *data, int size)
	{
		int result = ::recv(handle, static_cast<char *>(data), size, 0);
		if (result == -1)
		{
			if (errno == EWOULDBLOCK)
				return -1;
			else
				throw Exception("Error reading from server");
		}
		return result;
	}

	SocketName TCPConnectionImpl::local_name()
	{
		sockaddr_in addr;
		memset(&addr, 0, sizeof(sockaddr_in));
		socklen_t size = sizeof(sockaddr_in);
		int result = getsockname(handle, (sockaddr *)&addr, &size);
		if (result == -1)
			throw Exception("Error retrieving local socket name");

		SocketName name;
		name.from_sockaddr(AF_INET, (sockaddr *)&addr, sizeof(sockaddr_in));
		return name;
	}

	SocketName TCPConnectionImpl::remote_name()
	{
		sockaddr_in addr;
		memset(&addr, 0, sizeof(sockaddr_in));
		socklen_t size = sizeof(sockaddr_in);
		int result = getpeername(handle, (sockaddr *)&addr, &size);
		if (result == -1)
			throw Exception("Error retrieving remote socket name");

		SocketName name;
		name.from_sockaddr(AF_INET, (sockaddr *)&addr, sizeof(sockaddr_in));
		return name;
	}

#endif

	std::shared_ptr<TCPConnection> TCPConnection::connect(const SocketName &endpoint)
	{
		return std::make_shared<TCPConnectionImpl>(endpoint);
	}
}
