#pragma once
#include <TML/System/String.h>
#include <TML/Export.h>
#include <cstdint>
#include <streambuf>
#include <string>
#include <type_traits>

namespace tml
{
	class TML_API Serializer
	{
	public:
		Serializer() = default;
		virtual ~Serializer() = default;

		template<typename Serializable>
		Serializer& operator<<(const Serializable& data)
		{
			data.Serialize(*this);
			return *this;
		}

		Serializer& operator<<(bool data);
		Serializer& operator<<(int8_t data);
		Serializer& operator<<(int16_t data);
		Serializer& operator<<(int32_t data);
		Serializer& operator<<(int64_t data);

		Serializer& operator<<(uint8_t data);
		Serializer& operator<<(uint16_t data);
		Serializer& operator<<(uint32_t data);
		Serializer& operator<<(uint64_t data);

		Serializer& operator<<(float data);
		Serializer& operator<<(double data);
		Serializer& operator<<(const std::string& data);
		Serializer& operator<<(const String& data);

	private:
		virtual void OnSerialize(const void* data, std::streamsize length) = 0;
	};
}