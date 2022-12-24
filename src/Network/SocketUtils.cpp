#include "SocketUtils.h"
#include <TML/Network/IpAddress.h>
#include <cstring>

sockaddr_in MakeAddress(uint32_t port, const tml::IpAddress& address)
{
	return MakeAddress(address, port);
}

sockaddr_in MakeAddress(const tml::IpAddress& address, uint32_t port)
{
	sockaddr_in sockAddr{};
	memset(&sockAddr, 0, sizeof(sockAddr));

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(static_cast<int16_t>(port));
	sockAddr.sin_addr.s_addr = htonl(address.AsInteger());

#if defined(TML_PLATFORM_APPLE)
	sockAddr.sin_len = sizeof(sockAddr);
#endif

	return sockAddr;
}

sockaddr_in MakeAddress(uint32_t address, uint32_t port)
{
	sockaddr_in sockAddr{};
	memset(&sockAddr, 0, sizeof(sockAddr));

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(static_cast<int16_t>(port));
	sockAddr.sin_addr.s_addr = htonl(address);

#if defined(TML_PLATFORM_APPLE)
	sockAddr.sin_len = sizeof(sockAddr);
#endif

	return sockAddr;
}