#include <TML/Network/IpAddress.h>
#include "PlatformHeaders.h"

using namespace tml;

IpAddress::IpAddress()
: m_address(0)
{

}

IpAddress::IpAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
: m_address(htonl((a << 24) | (b << 16) | (c << 8) | d))
{

}

IpAddress::IpAddress(uint32_t address)
: m_address(htonl(address))
{

}

IpAddress::IpAddress(const std::string& address)
: m_address(0)
{
	if(!address.empty())
	{
		if(const auto ip = inet_addr(address.c_str()); ip != INADDR_NONE)
		{
			m_address = ip;
		}
		else
		{
			addrinfo info{};
			info.ai_family = AF_INET;

			addrinfo* result = nullptr;

			if((getaddrinfo(address.c_str(), nullptr, &info, &result) == 0) && result)
			{
				auto* sockaddrIn = reinterpret_cast<sockaddr_in*>(result->ai_addr);
				m_address = sockaddrIn->sin_addr.s_addr;
				freeaddrinfo(result);
			}
		}
	}
}

IpAddress::IpAddress(const char* address)
: IpAddress(std::string(address))
{

}

uint32_t IpAddress::AsInteger() const
{
	return ntohl(m_address);
}

std::string IpAddress::AsString() const
{
	in_addr inAddr{};
	inAddr.s_addr = m_address;

	return inet_ntoa(inAddr);
}