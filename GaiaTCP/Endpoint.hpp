#pragma once

#include <boost/asio.hpp>

namespace Gaia::TCP
{
	/// Represents the information of a socket, contains ip, port and other information.
	using Endpoint = boost::asio::ip::tcp::endpoint;

	/**
	 * @brief Make an endpoint object according the given ip address and port.
	 * @param ip Text of IP. Be cautious that hostname such as 'localhost' is not allowed here.
	 * @param port Port number.
	 * @return Endpoint object of the given IP and port.
	 */
	Endpoint MakeEndpoint(const std::string& ip, unsigned int port);

	/**
	 * @brief Make an endpoint object of '127.0.0.1' aka. 'localhost' on the given port.
	 * @param port Port number.
	 * @return Endpoint object of the '127.0.0.1' on the given port.
	 */
	Endpoint MakeLocalEndpoint(unsigned int port);
}
