#include "Connection.hpp"
#include <memory>

namespace Gaia::TCP
{
    /**
     * @brief This static class is used to established a connection to the given remote address.
     */
    class Connector
    {
    public:
        /**
         * @brief Establish a TCP connection to the given peer endpoint.
         * @param peer_endpoint The address of the TCP acceptor to connect.
         * @return An available instance of Connection class.
         * @throw boost::wrapexcept<boost::system::system_error> When connection can not be established.
         */
        [[nodiscard]] static Connection Connect(const Endpoint& peer_endpoint) noexcept(false);
    };
}
