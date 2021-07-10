#pragma once

#include "Connection.hpp"

#include <boost/asio.hpp>

namespace Gaia::TCP
{
    /**
     * @brief This class is used for accept a remote connection request.
     */
    class Acceptor
    {
    private:
        /// Record this acceptor is bound or not.
        bool Bound {false};
        /// The bound address. This acceptor will listen on this address during the Accept() function call.
        Endpoint BoundAddress;

    public:
        /// Default constructor which will not bind itself automatically.
        Acceptor() = default;
        /// Constructor which will automatically bind itself to a port on local address.
        explicit Acceptor(unsigned int port);
        /// Constructor which will automatically bind itself to the given address.
        explicit Acceptor(const Endpoint& local_address);

        /// Query whether this acceptor is bound to an address or not.
        [[nodiscard]] bool IsBound() const noexcept
        {
            return Bound;
        }

        /**
         * @brief Bind this Acceptor to a port on local address.
         * @param port The port on local address to bind.
         * @details This function actually does nothing but save this address for further Accept() function call.
         */
        void Bind(unsigned int port);
        /**
         * @brief Bind this Acceptor to the given address.
         * @param local_address
         * @details This function actually does nothing but save this address for further Accept() function call.
         */
        void Bind(const Endpoint& local_address);
        /**
         * @brief Wait for connection request and then establish a connection.
         * @return The established connection out of the request.
         * @throw boost::wrapexcept<boost::system::system_error> When connection can not be established.
         */
        virtual Connection Accept();
    };
}