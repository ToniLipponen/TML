#pragma once
#include <TML/System/Serializer/Deserializer.h>
#include <istream>

namespace tml
{
	class StreamDeserializer : public Deserializer
	{
	public:
		StreamDeserializer(std::istream& stream)
		: m_stream(stream)
		{

		}

	private:
		void OnDeserialize(void* data, std::streamsize length) override
		{
			m_stream.read(static_cast<char*>(data), length);
		}

	private:
		std::istream& m_stream;
	};
}