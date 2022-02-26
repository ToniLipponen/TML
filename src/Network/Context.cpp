#include <TML/Network/Context.h>

#define ASIO_STANDALONE
#include <asio/include/asio/io_context.hpp>

namespace tml
{
    namespace Net
    {
        Context::Context()
        {
            m_context = new asio::io_context;
        }

        Context::~Context()
        {
            delete reinterpret_cast<asio::io_context*>(m_context);
        }
    }
}