#pragma once
#include <TML/System/Serializer/StreamSerializer.h>
#include <fstream>

namespace tml
{
	class FileSerializer : public StreamSerializer
	{
	public:
		FileSerializer(std::streamsize bufferSize = 8192)
		: tml::StreamSerializer(m_stream, bufferSize)
		{

		}

		FileSerializer(const std::string& filename, std::streamsize bufferSize = 8192)
		: m_stream(filename, m_flags), tml::StreamSerializer(m_stream, bufferSize)
		{

		}

		~FileSerializer() override
		{
			tml::StreamSerializer::Flush();
		}

		bool Open(const std::string& filename)
		{
			m_stream.open(filename, m_flags);

			return m_stream.is_open() && m_stream.good();
		}

		void Close()
		{
			m_stream.close();
		}

	private:
		static constexpr auto m_flags = std::ios::binary | std::ios::out;
		std::ofstream m_stream;
	};
}
