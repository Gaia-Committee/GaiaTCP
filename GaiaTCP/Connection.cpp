#include "Connection.hpp"

#include <utility>

namespace Gaia::TCP
{
    /// Move constructor
    Connection::Connection(Connection && source)  noexcept
            : IOContext(std::move(source.IOContext)), IOSocket(std::move(source.IOSocket))
    {}

    /// Constructor which will take over an existing IO environment.
    Connection::Connection(std::unique_ptr<boost::asio::io_context> &&context, boost::asio::ip::tcp::socket &&socket)
            : IOContext(std::move(context)), IOSocket(std::move(socket))
    {}

	/// Disconnect from the current connection.
	void Connection::Disconnect()
	{
		IOSocket.close();
	}

	/// Read bytes from the connection.
	ByteUtility::BytesBuffer Connection::Read()
	{
		std::vector<unsigned char> data(DefaultBufferSize);
		std::size_t received_size = IOSocket.read_some(boost::asio::buffer(data));
		data.resize(received_size);

		return data;
	}

	/// Read the given amount of bytes from the connection.
	ByteUtility::BytesBuffer Connection::Read(std::size_t length)
	{
		ByteUtility::BytesBuffer data(length);
		std::size_t received_size = 0;

		while (received_size < length)
		{
			received_size += IOSocket.read_some(boost::asio::buffer(data));
		}

		return data;
	}

	/// Write bytes to the connection.
	void Connection::Write(const ByteUtility::BytesAddress &bytes)
	{
		boost::asio::write(IOSocket, boost::asio::buffer(bytes.Data, bytes.Length));
	}
}