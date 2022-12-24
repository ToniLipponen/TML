#include <TML/Network/Packet.h>
#include <TML/Network/Hton.h>
#include <TML/System/Platform.h>
#include <cstring>

#ifdef TML_PLATFORM_UNIX
    #include <netinet/in.h>
    #include <sys/types.h>

#elif defined(TML_PLATFORM_WINDOWS)
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <winsock2.h>
#endif

namespace tml
{
    Packet::Packet()
    : m_readPos(0)
    {

    }

    Packet::Packet(const tml::Packet& other) = default;

    Packet::Packet(tml::Packet&& other) noexcept
    {
        m_data = std::move(other.m_data);
        other.m_readPos = 0;
    }

    Packet& Packet::operator=(const Packet& other) = default;

    Packet& Packet::operator=(Packet&& other) noexcept
    {
        m_data = std::move(other.m_data);
        other.m_readPos = 0;

        return *this;
    }

    void Packet::PushData(const void* data, uint64_t size)
    {
        auto oldSize = m_data.size();
        m_data.resize(oldSize + size);
        memcpy(m_data.data() + oldSize, data, size);
    }

    void Packet::ClearData()
    {
        m_data.clear();
        m_readPos = 0;
    }

    const void* Packet::GetData() const
    {
        return m_data.data();
    }

    uint64_t Packet::GetSize() const
    {
        return static_cast<int64_t>(m_data.size());
    }

    [[maybe_unused]]
    uint64_t Packet::GetReadBytes() const
    {
        return m_readPos;
    }

	bool Packet::HasData()
	{
		return !m_data.empty() && m_readPos < GetSize();
	}

    bool Packet::CheckLimits(uint64_t size)
    {
        return (m_readPos + size) <= m_data.size();
    }

	void Packet::OnDeserialize(void* data, std::streamsize length)
	{
		if(CheckLimits(length))
		{
			memcpy(data, m_data.data() + m_readPos, length);
			m_readPos += length;
		}
	}

	void Packet::OnSerialize(const void* data, std::streamsize length)
	{
		PushData(data, static_cast<uint64_t>(length));
	}
}