#pragma once
#include <TML/System/NonCopyable.h>
#include <TML/System/Serializer/Serializer.h>
#include <ostream>

namespace tml
{
	class StreamSerializer : public Serializer, NonCopyable
	{
	public:
		StreamSerializer(std::ostream& stream, std::streamsize bufferCapacity = 8192)
		: m_stream(stream), m_bufferLength(0), m_bufferCapacity(bufferCapacity)
		{
			m_buffer.resize(m_bufferCapacity);
		}

		~StreamSerializer() override
		{
			Flush();
		}

	private:
		void OnSerialize(const void* data, std::streamsize length) override
		{
			if((length + m_bufferLength) > m_bufferCapacity)
			{
				Flush();
			}

			if(length > m_bufferCapacity)
			{
				m_stream.write(static_cast<const char*>(data), length);
			}
			else if(length <= (m_bufferCapacity - m_bufferLength))
			{
				memcpy(&m_buffer[m_bufferLength], data, length);
				m_bufferLength += length;
			}
		}

	protected:
		void Flush()
		{
			m_stream.write(m_buffer.data(), m_bufferLength);
			m_bufferLength = 0;
		}

	private:
		std::ostream& m_stream;
		std::vector<char> m_buffer;
		std::streamsize m_bufferLength = 0, m_bufferCapacity = 0;
	};
}