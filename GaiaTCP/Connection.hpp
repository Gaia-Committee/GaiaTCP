#pragma once

#include "Endpoint.hpp"
#include <GaiaByteUtility/GaiaByteUtility.hpp>
#include <boost/asio.hpp>
#include <memory>

namespace Gaia::TCP
{
    class Connector;
    class Acceptor;

	/**
	 * @brief It represents the connection between two ports on TCP protocol.
	 * @details Pay attention that a exception of 'boost::wrapexcept<boost::system::system_error>' will be thrown when
	 *          the connection is broken without being closed before.
	 */
	class Connection
	{
	    friend class Connector;
	    friend class Acceptor;

	protected:
		/// Context for IO jobs, declared as a automatic pointer type because boost::asio::io_context can not be moved.
		std::unique_ptr<boost::asio::io_context> IOContext;
		/// Socket for communication.
		boost::asio::ip::tcp::socket IOSocket;

		/**
		 * @brief Constructor which will take over an IO environment,
		 *        including a context and a socket.
		 * @param context A moved unique_ptr of an IO context object.
		 * @param socket A moved IO socket.
		 */
		Connection(decltype(IOContext)&& context, decltype(IOSocket)&& socket);

	public:
        /// Move constructor.
        Connection(Connection&&) noexcept;
	    /// Copying a connection is logically confusing.
	    Connection(Connection&) = delete;

		/// The default size of the buffer when it's not explicitly given.
		std::size_t DefaultBufferSize {512};

		/**
		 * @brief Query whether the connection is valid or not.
		 * @retval true The connection functions properly.
		 * @retval false The connection has been broken.
		 * @throw boost::wrapexcept<boost::system::system_error> When any operation failed.
		 */
		[[nodiscard]] bool IsValid() const
		{
			return IOSocket.is_open();
		}

		/**
		 * @brief Disconnect the current connection.
		 * @pre Already connected to a remote address.
		 * @throw boost::wrapexcept<boost::system::system_error> When any operation failed.
		 */
		virtual void Disconnect() noexcept(false);

		/**
		 * @brief Read bytes from the buffer until meet the end of the transmission data or the end of the buffer.
		 * @pre Already connected.
		 * @return A bytes vector of the transmission data.
		 * @throw boost::wrapexcept<boost::system::system_error>r When any operation failed.
		 */
		ByteUtility::BytesBuffer Read() noexcept(false);

		/**
		 * @brief Read a specific amount of data from the buffer, this function block the call until read the required
		 *        amount of bytes.
		 * @param length The specified length of data to read.
		 * @pre Already connected.
		 * @return A bytes vector of the required length of the transmission data.
		 * @throw boost::system::system_error When any operation failed.
		 */
		ByteUtility::BytesBuffer Read(std::size_t length) noexcept(false);

		/**
		 * @brief Write some data into the Channel`s buffer.
		 * @param bytes Bytes to send.
		 * @pre Already connected.
		 * @throw boost::system::system_error When any operation failed.
		 */
		void Write(const ByteUtility::BytesAddress& bytes) noexcept(false);
	};
}