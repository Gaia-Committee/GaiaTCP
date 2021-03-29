#include "Endpoint.hpp"

namespace Gaia::TCP
{
	/// Make an endpoint object according the given ip address and port.
	Endpoint MakeEndpoint(const std::string& ip, unsigned int port)
    {
        return boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port);
	}

	/// Make an endpoint object of the local ip and the given port.
	Endpoint MakeLocalEndpoint(unsigned int port)
	{
		return boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port);
	}
}