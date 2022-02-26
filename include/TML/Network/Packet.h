#pragma once
#include <TML/Types.h>
#include <cstring>
#include <vector>

#include "Connection.h"

namespace tml
{
    namespace Net
    {
        class Packet
        {
        public:
            struct Header
            {
                ui32 id;
                ui32 size;
            };
        public:
            Packet(){}
            void ClearData(){}
            void PushData(ui32 bytes, ui8* data)
            {
                const auto size = m_data.size();
                m_data.resize(size + bytes);
                std::memcpy(&m_data[0] + size, data, bytes);
                m_header.size = m_data.size() + sizeof(ui32);
            }
            const std::vector<ui8>& GetData() const noexcept
            {
                return m_data;
            }
        private:
            Header m_header{};
            std::vector<ui8> m_data;
        };

        struct IdentityPacket
        {
            Packet packet;
            class Connection* m_sender = nullptr;
        };
    }
}