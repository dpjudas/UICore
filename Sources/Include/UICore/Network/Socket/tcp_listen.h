
#pragma once

#include "network_condition_variable.h"
#include <memory>
#include <vector>

namespace uicore
{
	class SocketName;
	class TCPConnection;

	/// \brief Listens for incoming TCP/IP socket connections
	class TCPListen : public NetworkEvent
	{
	public:
		/// \brief Create a listening socket for the specified end point
		static std::shared_ptr<TCPListen> listen(const SocketName &endpoint, int backlog = 5, bool reuse_address = true);

		/// \brief Stops listening for incoming messages and closes the socket
		virtual void close() = 0;

		/// \brief Accept an incoming connection
		/// \param end_point Socket name of the accepted peer
		/// \return A TcpConnection for the accepted connection, or null if no incoming connections are available
		virtual std::shared_ptr<TCPConnection> accept(SocketName &end_point) = 0;
	};

	typedef std::shared_ptr<TCPListen> TCPListenPtr;
}
