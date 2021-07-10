#include "Connection.hpp"

#include <utility>

namespace Gaia::TCP
{
    /// Move constructor
    Connection::Connection(Connection && source)  noexcept :
            IOContext(std::move(source.IOContext)), IOSocket(std::move(source.IOSocket)),
            IOWorker(std::move(source.IOWorker))
    {}

    /// Constructor which will take over an existing IO environment.
    Connection::Connection(std::unique_ptr<boost::asio::io_context> &&context, boost::asio::ip::tcp::socket &&socket) :
            IOContext(std::move(context)), IOSocket(std::move(socket)),
            IOWorker([this](const std::atomic_bool& flag){
            while (flag)
            {
                this->IOContext->run();
//                this->IOContext->run_for(std::chrono::milliseconds(500));
            }
        })
    {}

    /// Auto disconnect if opened
    Connection::~Connection()
    {
        if (IOWorker.IsRunning()) IOWorker.Stop();
        if (IOSocket.is_open())
        {
            IOSocket.close();
        }
    }

	/// Disconnect from the current connection.
	void Connection::Disconnect()
	{
        if (IOWorker.IsRunning()) IOWorker.Stop();
		IOSocket.close();
	}

	/// Read bytes from the buffer.
    std::size_t Connection::Read(char *buffer, std::size_t buffer_size, bool until_full)
    {
        std::size_t result;
        if (!until_full)
        {
            result = IOSocket.read_some(boost::asio::buffer(buffer, buffer_size));
        }
        else
        {
            result = boost::asio::read(IOSocket, boost::asio::buffer(buffer, buffer_size),
                                     boost::asio::transfer_exactly(buffer_size));
        }
        return result;
    }

    /// Write some data into socket.
    void Connection::Write(const char *buffer, std::size_t length)
    {
        boost::asio::write(IOSocket, boost::asio::buffer(buffer, length), boost::asio::transfer_all());
    }

    /// Write a simple string into the socket.
    void Connection::Write(const std::string& data)
    {
        Write(data.data(), data.size());
    }

    /// Enter listener mode.
    void Connection::StartListen()
    {
        AddListenTask();
        IOWorker.Start();
    }

    /// Exit listener mode.
    void Connection::StopListen()
    {
        IOWorker.Stop();
    }

    /// Add listen task to the io service, the task can add itself when it finishes.
    void Connection::AddListenTask()
    {
        auto buffer = std::make_shared<std::string>();
        buffer->resize(this->ReceiverBufferSize);
        IOSocket.async_read_some(boost::asio::buffer(buffer->data(), buffer->size()),
        [this, passed_buffer = buffer](
                const boost::system::error_code& error, std::size_t length)
        {
            if (error) return;
            passed_buffer->resize(length);
            this->OnReceive.Trigger(*passed_buffer);
            this->AddListenTask();
        });
    }

    /// Block the invoker thread until error occurs.
    void Connection::WaitError()
    {
        IOSocket.wait(boost::asio::socket_base::wait_error);
    }
}