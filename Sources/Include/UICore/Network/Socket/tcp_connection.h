
#pragma once

#include "network_condition_variable.h"

#include <memory>
#include <vector>
#include <mutex>

namespace uicore
{
	class SocketName;

	/// \brief TCP/IP socket connection
	class TCPConnection : public NetworkEvent
	{
	public:
		/// \brief Blocking connect to end point
		static std::shared_ptr<TCPConnection> connect(const SocketName &endpoint);

		/// \brief Returns the socket name of the local end point
		virtual SocketName local_name() = 0;

		/// \brief Returns the socket name of the peer end point
		virtual SocketName remote_name() = 0;

		/// \brief Close connection
		virtual void close() = 0;

		/// \brief Write data to TCP socket
		/// \return Bytes written, or -1 if buffer is full
		virtual int write(const void *data, int size) = 0;

		/// \brief Read data from TCP socket
		/// \return Bytes read, 0 if remote closed connection, or -1 if buffer is empty
		virtual int read(void *data, int size) = 0;

		// To do: QOSAddSocketToFlow
	};

	typedef std::shared_ptr<TCPConnection> TCPConnectionPtr;
}
