#pragma once
#include "PlatformHeaders.h"
#include <TML/Network/IpAddress.h>
#include <optional>

sockaddr_in MakeAddress(uint32_t port, const tml::IpAddress& address = tml::IpAddress(0,0,0,0));
sockaddr_in MakeAddress(const tml::IpAddress& address, uint32_t port);
sockaddr_in MakeAddress(uint32_t address, uint32_t port);