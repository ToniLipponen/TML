#include <TML/System/Serializer/Deserializer.h>
#include <TML/Network/Hton.h>

using namespace tml;

Deserializer& Deserializer::operator>>(bool& data)
{
	OnDeserialize(&data, sizeof(data));

	return *this;
}

Deserializer& Deserializer::operator>>(int8_t& data)
{
	OnDeserialize(&data, sizeof(data));

	return *this;
}

Deserializer& Deserializer::operator>>(int16_t& data)
{
	OnDeserialize(&data, sizeof(data));
	data = static_cast<int16_t>(Ntohs(static_cast<uint16_t>(data)));

	return *this;
}

Deserializer& Deserializer::operator>>(int32_t& data)
{
	OnDeserialize(&data, sizeof(data));
	data = static_cast<int32_t>(Ntohl(static_cast<uint32_t>(data)));

	return *this;
}

Deserializer& Deserializer::operator>>(int64_t& data)
{
	OnDeserialize(&data, sizeof(data));
	data = static_cast<int64_t>(Ntohll(static_cast<uint64_t>(data)));

	return *this;
}

Deserializer& Deserializer::operator>>(uint8_t& data)
{
	OnDeserialize(&data, sizeof(data));

	return *this;
}

Deserializer& Deserializer::operator>>(uint16_t& data)
{
	OnDeserialize(&data, sizeof(data));
	data = Ntohs(data);

	return *this;
}

Deserializer& Deserializer::operator>>(uint32_t& data)
{
	OnDeserialize(&data, sizeof(data));
	data = Ntohl(data);

	return *this;
}

Deserializer& Deserializer::operator>>(uint64_t& data)
{
	OnDeserialize(&data, sizeof(data));
	data = Ntohll(data);

	return *this;
}

Deserializer& Deserializer::operator>>(float& data)
{
	uint32_t temp;
	OnDeserialize(&temp, sizeof(temp));
	data = Ntohf(temp);

	return *this;
}

Deserializer& Deserializer::operator>>(double& data)
{
	uint64_t temp;
	OnDeserialize(&temp, sizeof(temp));
	data = Ntohd(temp);

	return *this;
}

Deserializer& Deserializer::operator>>(std::string& data)
{
	uint64_t length = 0;
	OnDeserialize(&length, sizeof(length));
	length = Ntohll(length);

	data.resize(length);
	OnDeserialize(&data[0], static_cast<std::streamsize>(length));

	return *this;
}

Deserializer& Deserializer::operator>>(String& data)
{
	uint64_t length = 0;
	OnDeserialize(&length, sizeof(length));
	length = Ntohll(length);

	data.clear();

	for(uint64_t i = 0; i < length; i++)
	{
		char32_t c = 0;
		OnDeserialize(&c, sizeof(c));
		c = Ntohl(c);
		data.push_back(c);
	}

	return *this;
}