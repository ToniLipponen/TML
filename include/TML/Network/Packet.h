#pragma once
#include <TML/Export.h>
#include <TML/System/String.h>
#include <TML/System/Serializer/Serializer.h>
#include <TML/System/Serializer/Deserializer.h>
#include <vector>
#include <cstdint>
#include <string>

namespace tml
{
    class TML_API Packet : public Serializer, public Deserializer
    {
    public:
        Packet();
        Packet(const Packet& other);
        Packet(Packet&& other) noexcept;

        Packet& operator=(const Packet& other);
        Packet& operator=(Packet&& other) noexcept;

        void PushData(const void* data, uint64_t size);
        void ClearData();

        [[nodiscard]] const void* GetData() const;
        [[nodiscard]] uint64_t GetSize() const;
        [[nodiscard]] uint64_t GetReadBytes() const;
		[[nodiscard]] bool HasData();

        friend class Socket;
    private:
        bool CheckLimits(uint64_t size);
		void OnDeserialize(void* data, std::streamsize length) override;
		void OnSerialize(const void* data, std::streamsize length) override;

    private:
        std::vector<char> m_data;
        uint64_t m_readPos = 0;
    };
}