#pragma once
#include <TML/Export.h>
#include <cstdint>
#include <string>

namespace tml
{
	class TML_API IpAddress
	{
	public:
		IpAddress();
		IpAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
		IpAddress(uint32_t address);
		IpAddress(const std::string& address);
		IpAddress(const char* address);

		[[nodiscard]] uint32_t AsInteger() const;
		[[nodiscard]] std::string AsString() const;

	private:
		uint32_t m_address;
	};
}