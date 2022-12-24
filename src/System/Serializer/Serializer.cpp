#include <TML/System/Serializer/Serializer.h>
#include <TML/Network/Hton.h>

using namespace tml;

Serializer& Serializer::operator<<(bool data)
{
	OnSerialize(&data, sizeof(data));

	return *this;
}

Serializer& Serializer::operator<<(int8_t data)
{
	OnSerialize(&data, sizeof(data));

	return *this;
}

Serializer& Serializer::operator<<(int16_t data)
{
	auto converted = static_cast<int16_t>(Htons(static_cast<uint16_t>(data)));
	OnSerialize(&converted, sizeof(converted));

	return *this;
}

Serializer& Serializer::operator<<(int32_t data)
{
	auto converted = static_cast<int32_t>(Htonl(static_cast<uint32_t>(data)));
	OnSerialize(&converted, sizeof(converted));

	return *this;
}

Serializer& Serializer::operator<<(int64_t data)
{
	auto converted = static_cast<int64_t>(Htonll(static_cast<uint64_t>(data)));
	OnSerialize(&converted, sizeof(converted));

	return *this;
}

Serializer& Serializer::operator<<(uint8_t data)
{
	OnSerialize(&data, sizeof(data));

	return *this;
}

Serializer& Serializer::operator<<(uint16_t data)
{
	auto converted = Htons(data);
	OnSerialize(&converted, sizeof(converted));

	return *this;
}

Serializer& Serializer::operator<<(uint32_t data)
{
	auto converted = Htonl(data);
	OnSerialize(&converted, sizeof(converted));

	return *this;
}

Serializer& Serializer::operator<<(uint64_t data)
{
	auto converted = Htonll(data);
	OnSerialize(&converted, sizeof(converted));

	return *this;
}

Serializer& Serializer::operator<<(float data)
{
	auto converted = Htonf(data);
	OnSerialize(&converted, sizeof(converted));

	return *this;
}

Serializer& Serializer::operator<<(double data)
{
	auto converted = Htond(data);
	OnSerialize(&converted, sizeof(converted));

	return *this;
}

Serializer& Serializer::operator<<(const std::string& data)
{
	auto length = Htonll(data.size());
	OnSerialize(&length, sizeof(length));
	OnSerialize(data.data(), static_cast<std::streamsize>(data.size()));

	return *this;
}

Serializer& Serializer::operator<<(const String& data)
{
	auto length = Htonll(static_cast<uint64_t>(data.length()));
	OnSerialize(&length, sizeof(length));

	for(auto c : data)
	{
		auto converted = Htonl(c);
		OnSerialize(&converted, sizeof(converted));
	}

	return *this;
}