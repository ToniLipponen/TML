#pragma once
#include <cstdint>

namespace tml
{
	inline uint64_t Htonll(uint64_t value)
	{
		unsigned char bytes[sizeof(value)];
		memcpy(&bytes[0], &value, sizeof(value));

		return  (static_cast<uint64_t>(bytes[0]) << 56) |
				(static_cast<uint64_t>(bytes[1]) << 48) |
				(static_cast<uint64_t>(bytes[2]) << 40) |
				(static_cast<uint64_t>(bytes[3]) << 32) |
				(static_cast<uint64_t>(bytes[4]) << 24) |
				(static_cast<uint64_t>(bytes[5]) << 16) |
				(static_cast<uint64_t>(bytes[6]) <<  8) |
				static_cast<uint64_t>(bytes[7]);
	}

	inline uint32_t Htonl(uint32_t value) noexcept
	{
		unsigned char bytes[sizeof(value)];
		memcpy(&bytes[0], &value, sizeof(value));

		return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] <<  8) | (bytes[3] <<  0);
	}

	inline uint16_t Htons(uint16_t value) noexcept
	{
		unsigned char bytes[sizeof(value)];
		memcpy(&bytes[0], &value, sizeof(value));

		return (bytes[0] << 8) | bytes[1];
	}

	inline uint32_t Htonf(float value) noexcept
	{
		uint32_t temp;
		memcpy(&temp, &value, sizeof(value));

		return Htonl(temp);
	}

	inline uint64_t Htond(double value) noexcept
	{
		uint64_t temp;
		memcpy(&temp, &value, sizeof(value));

		return Htonll(temp);
	}

	inline uint64_t Ntohll(uint64_t value) noexcept
	{
		return Htonll(value);
	}

	inline uint32_t Ntohl(uint32_t value) noexcept
	{
		return Htonl(value);
	}

	inline uint16_t Ntohs(uint16_t value) noexcept
	{
		return Htons(value);
	}

	inline float Ntohf(uint32_t value) noexcept
	{
		auto temp = Htonl(value);

		float returnValue;
		memcpy(&returnValue, &temp, sizeof(returnValue));

		return returnValue;
	}

	inline double Ntohd(uint64_t value) noexcept
	{
		auto temp = Htonll(value);

		double returnValue;
		memcpy(&returnValue, &temp, sizeof(returnValue));

		return returnValue;
	}
}