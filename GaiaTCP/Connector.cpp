#include "Connector.hpp"

namespace Gaia::TCP
{
    /// Establish a connection to a peer endpoint.
    Connection Connector::Connect(const Endpoint& peer_endpoint) noexcept(false)
    {
        auto context = std::make_unique<boost::asio::io_context>();
        boost::asio::ip::tcp::socket socket(*context);
        socket.connect(peer_endpoint);
        return Connection(std::move(context), std::move(socket));
    }
}