#pragma once
#include <TML/System/Serializer/StreamDeserializer.h>
#include <fstream>

namespace tml
{
	class FileDeserializer : public StreamDeserializer
	{
	public:
		FileDeserializer()
		: StreamDeserializer(m_stream)
		{

		}

		FileDeserializer(const std::string& filename)
		: m_stream(filename), StreamDeserializer(m_stream)
		{

		}

		bool Open(const std::string& filename)
		{
			m_stream.open(filename);

			return m_stream.is_open() && m_stream.good();
		}

		void Close()
		{
			m_stream.close();
		}

	private:
		static constexpr auto m_flags = std::ios::binary | std::ios::in;
		std::ifstream m_stream;
	};
}
