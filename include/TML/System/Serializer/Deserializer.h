#pragma once
#include <TML/System/String.h>
#include <TML/Export.h>
#include <streambuf>
#include <string>
#include <type_traits>

namespace tml
{
	class TML_API Deserializer
	{
	public:
		Deserializer() = default;
		virtual ~Deserializer() = default;

		template<typename Deserializable>
		Deserializer& operator>>(Deserializable& data)
		{
			data.Deserialize(*this);
			
			return *this;
		}

		Deserializer& operator>>(bool& data);
		Deserializer& operator>>(int8_t& data);
		Deserializer& operator>>(int16_t& data);
		Deserializer& operator>>(int32_t& data);
		Deserializer& operator>>(int64_t& data);

		Deserializer& operator>>(uint8_t& data);
		Deserializer& operator>>(uint16_t& data);
		Deserializer& operator>>(uint32_t& data);
		Deserializer& operator>>(uint64_t& data);

		Deserializer& operator>>(float& data);
		Deserializer& operator>>(double& data);
		Deserializer& operator>>(std::string& data);
		Deserializer& operator>>(String& data);

	private:
		virtual void OnDeserialize(void* data, std::streamsize length) = 0;
	};
}
