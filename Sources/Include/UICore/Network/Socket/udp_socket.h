
#pragma once

#include "network_condition_variable.h"
#include <memory>
#include <vector>

namespace uicore
{
	class SocketName;

	/// \brief UDP/IP socket class
	class UDPSocket : public NetworkEvent
	{
	public:
		/// \brief Create socket object
		static std::shared_ptr<UDPSocket> create();

		/// \brief Close connection
		virtual void close() = 0;

		/// \brief Bind socket to end point
		virtual void bind(const SocketName &endpoint) = 0;

		/// \brief Send UDP packet to end point
		virtual void send(const void *data, int size, const SocketName &endpoint) = 0;

		/// \brief Read receved UDP packet
		/// \return Bytes read or 0 if no packet was available
		virtual int read(void *data, int size, SocketName &endpoint) = 0;
	};

	typedef std::shared_ptr<UDPSocket> UDPSocketPtr;
}
