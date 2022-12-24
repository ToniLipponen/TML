#pragma once
#include <TML/System/Platform.h>
#include <TML/System/NonCopyable.h>
#include <TML/Network/Packet.h>
#include <TML/Network/IpAddress.h>
#include <string>
#include <cstdint>

namespace tml
{
    class TML_API Socket : NonCopyable
    {
	public:
#if defined(TML_PLATFORM_UNIX)
		using DescriptorType = int32_t;
#else
		using DescriptorType = uint64_t;
#endif
		enum class Type
		{
			Tcp,
			Udp
		};

		enum class Result
		{
			OK,                 //!< All good
			Incomplete,
			Disconnected,
			Error,
		};

	protected:
		explicit Socket(Type type);

    public:
		virtual ~Socket();

		void Create();
		void Close();

		virtual Result Connect(const IpAddress& address, uint32_t port) = 0;

        virtual Result Send(const void* data, uint64_t size, uint64_t& sent) = 0;
        virtual Result Send(const Packet& packet) = 0;

        virtual Result Receive(void* data, uint64_t size, uint64_t& received) = 0;
        virtual Result Receive(Packet& packet) = 0;

        Result SetBlocking(bool blocking);
        [[nodiscard]] bool IsBlocking() const; //!< Is the socket blocking?
        [[nodiscard]] DescriptorType GetHandle() const;

		friend class TcpReceiver;

    protected:
		static Result CheckErrno();

    protected:
        bool m_blocking = true;
		DescriptorType m_fd;
		Type m_type;
    };
}