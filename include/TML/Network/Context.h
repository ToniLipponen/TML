#pragma once
#include <memory>
namespace tml
{
    namespace Net
    {
        class Context
        {
        public:
            Context();
            virtual ~Context();

            friend class Socket;
        protected:
            void* m_context = nullptr;
        };
    }
}