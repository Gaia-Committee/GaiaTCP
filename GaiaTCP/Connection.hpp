#pragma once

#include "Endpoint.hpp"
#include <GaiaEvents/GaiaEvents.hpp>
#include <GaiaBackground/GaiaBackground.hpp>
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
		std::unique_ptr<boost::asio::io_service> IOContext;
		/// Socket for communication.
		boost::asio::ip::tcp::socket IOSocket;
        //// The background worker which will run IOContext function run_for(...).
        Background::BackgroundWorker IOWorker;
		/**
		 * @brief Constructor which will take over an IO environment,
		 *        including a context and a socket.
		 * @param context A moved unique_ptr of an IO context object.
		 * @param socket A moved IO socket.
		 */
		Connection(decltype(IOContext)&& context, decltype(IOSocket)&& socket);

	protected:
	    /// Add listen task to the io service, the task can add itself when it finishes.
	    void AddListenTask();

	public:
        /// Move constructor.
        Connection(Connection&&) noexcept;
	    /// Copying a connection is logically confusing.
	    Connection(Connection&) = delete;

	    /// Auto disconnect if opened.
	    virtual ~Connection();

		/**
		 * @brief Query whether the connection is opened or not.
		 * @retval true The connection functions properly.
		 * @retval false The connection has been broken.
		 * @throw boost::wrapexcept<boost::system::system_error> When any operation failed.
		 */
		[[nodiscard]] bool IsOpened() const
		{
			return IOSocket.is_open();
		}

		/**
		 * @brief Disconnect the current connection, and stop the running listener.
		 * @pre Already connected to a remote address.
		 * @throw boost::wrapexcept<boost::system::system_error> When any operation failed.
		 */
		virtual void Disconnect() noexcept(false);

		/**
		 * @brief Read bytes from the buffer, if required_size is given a positive value,
		 *        this function block the call until buffer is full.
		 * @param buffer Address of buffer to store received bytes.
		 * @param buffer_size Size of the buffer.
		 * @param until_full If true, this function will only return after the buffer is full.
		 * @pre Already connected.
		 * @return Count of received bytes.
		 * @throw boost::system::system_error When any operation failed.
		 */
		std::size_t Read(char* buffer, std::size_t buffer_size, bool until_full = false);

		/**
		 * @brief Write some data into socket.
		 * @param bytes Bytes to send.
		 * @pre Already connected.
		 * @throw boost::system::system_error When any operation failed.
		 */
		void Write(const char *buffer, std::size_t length);

		/// Write a simple string into the socket.
		void Write(const std::string& data);

		/// Triggered when receive a message.
		Events::Event<const std::string&> OnReceive;
		/// Buffer size of OnReceive event trigger.
		std::size_t ReceiverBufferSize {256};

		/**
		 * @brief Enter listener mode.
		 * @details When data arrived, OnReceive event will be triggered.
		 */
		void StartListen();
        /**
         * @brief Exit listener mode.
         * @details After this function call, new arrived data will not trigger OnReceive event.
         */
		void StopListen();

        /// Block the invoker thread until error occurs.
		void WaitError();
	};
}