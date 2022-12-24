#include <TML/Network/SocketMonitor.h>
#include "PlatformHeaders.h"
using namespace tml;

struct SocketMonitor::Data
{
	std::vector<pollfd> descriptors;
};

SocketMonitor::SocketMonitor()
: m_data(new SocketMonitor::Data)
{

}

SocketMonitor::~SocketMonitor() = default;

void SocketMonitor::Add(const tml::Socket& socket)
{
	auto handle = socket.GetHandle();

	for(auto& i : m_data->descriptors)
	{
		if(i.fd == handle)
		{
			return;
		}
	}

	pollfd pollStruct{};
	pollStruct.fd = handle;
	pollStruct.events = POLLIN | POLLHUP | POLLOUT | POLLPRI;
	pollStruct.revents = 0;

	m_data->descriptors.push_back(pollStruct);
}

void SocketMonitor::Remove(const tml::Socket& socket)
{
	auto handle = socket.GetHandle();

	for(auto i = m_data->descriptors.begin(); i < m_data->descriptors.end();)
	{
		if(i->fd == handle)
		{
			m_data->descriptors.erase(i);
			return;
		}

		++i;
	}
}

bool SocketMonitor::Poll(int32_t timeout)
{
	int result;

#if defined(TML_PLATFORM_UNIX)
	result = poll(m_data->descriptors.data(), m_data->descriptors.size(), timeout);
#elif defined(TML_PLATFORM_WINDOWS)
	result = WSAPoll(m_data->descriptors.data(), m_data->descriptors.size(), timeout);
#endif

	if(result > 0)
	{
		return true;
	}
	else if(result < 0)
	{
		std::cerr << "Failed to poll\n";
	}

	return false;
}

bool tml::SocketMonitor::CanReceive(const Socket& socket) const
{
	auto handle = socket.GetHandle();

	for(auto& i : m_data->descriptors)
	{
		if(i.fd == handle)
		{
			return (i.revents & (POLLIN | POLLPRI)) != 0;
		}
	}

	return false;
}

bool tml::SocketMonitor::CanSend(const Socket& socket) const
{
	auto handle = socket.GetHandle();

	for(auto& i : m_data->descriptors)
	{
		if(i.fd == handle)
		{
			return (i.revents & POLLOUT) != 0;
		}
	}

	return false;
}

bool tml::SocketMonitor::HasDisconnected(const Socket& socket) const
{
	auto handle = socket.GetHandle();

	for(auto& i : m_data->descriptors)
	{
		if(i.fd == handle)
		{
			return (i.revents & (POLLHUP)) != 0;
		}
	}

	return false;
}
