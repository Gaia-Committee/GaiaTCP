#include "Acceptor.h"

#include <utility>
#include <stdexcept>
#include <GaiaExceptions/GaiaExceptions.hpp>

namespace Gaia::TCP
{
    /// Constructor which will bind itself to a port on local address.
    Acceptor::Acceptor(unsigned int port)
    {
        Bind(port);
    }

    /// Constructor which will bind itself to the given address.
    Acceptor::Acceptor(const Endpoint& local_address)
    {
        Bind(local_address);
    }

    /// Bind this acceptor to a port on the local address.
    void Acceptor::Bind(unsigned int port)
    {
        Bind(MakeLocalEndpoint(port));
    }

    /// Bind this acceptor to the given address.
    void Acceptor::Bind(const Endpoint& local_address)
    {
        Bound = true;
        BoundAddress = local_address;
    }

    /// Accept a request and establish a connection.
    Connection Acceptor::Accept()
    {
        /**
         * The context is newly constructed here,
         * because IO objects of boost::asio in the same context will actually work in serial working model.
         */
        auto context = std::make_unique<boost::asio::io_context>();
        auto socket = boost::asio::ip::tcp::socket(*context);
        boost::asio::ip::tcp::acceptor acceptor(*context, BoundAddress);
        acceptor.accept(socket);
        return Connection(std::move(context), std::move(socket));
    }
}