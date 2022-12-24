#pragma once
#include <TML/Network/Socket.h>
#include <TML/Export.h>
#include <memory>

namespace tml
{
	class TML_API SocketMonitor : NonCopyable
	{
	public:
		SocketMonitor();
		~SocketMonitor();

		void Add(const Socket& socket);
		void Remove(const Socket& socket);

		bool Poll(int32_t timeout = 0);
		[[nodiscard]] bool CanReceive(const Socket& socket) const;
		[[nodiscard]] bool CanSend(const Socket& socket) const;
		[[nodiscard]] bool HasDisconnected(const Socket& socket) const;

	private:
		struct Data;
		std::unique_ptr<Data> m_data;
	};
}