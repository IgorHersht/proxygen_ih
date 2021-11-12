#pragma once

#include <memory>
#include <boost/asio.hpp>
#include <common/message.h>
#include <common/request_handler.h>
#include <utils/utils.h>

namespace network {
namespace server {
    using   HandlerThreadPoolPtr = std::shared_ptr<boost::asio::thread_pool>;

/// Represents a single Connection from a client.
struct Connection : public std::enable_shared_from_this<Connection>
{
  /// Construct a Connection with the given ioContext.
  Connection(boost::asio::io_context& ioContext, HandlerThreadPoolPtr handlerThreadPool, RequestHandlerPtr requestHandler);
  Connection(const Connection&) = delete;
  Connection& operator =(const Connection&) = delete;
  /// Get the socket associated with the Connection.
  boost::asio::ip::tcp::socket& socket();
  /// Start the first asynchronous operation for the Connection.
  void start();
  void stop(const boost::system::error_code& e = boost::system::error_code() );
private:
    friend class RequestHandlerCallback;
    void readHeader();
  /// Handlers completion of a read operations.
  void handleReadHeader(const boost::system::error_code& e, std::size_t bytesTransferred);
  void readBody();
  void handleReadBody(const boost::system::error_code& e, std::size_t bytesTransferred);
  void onRequestHandled();
  void write();
  /// Handle completion of a write operation.
  void handleWrite(const boost::system::error_code& e, std::size_t bytesTransferred);
private:
  /// Socket for the Connection.
  boost::asio::ip::tcp::socket      _socket;
  HandlerThreadPoolPtr              _handlerThreadPool;
  /// buffers
  BufferPtr                           _requestHeaderBufferPtr;
  BufferPtr                           _requestBodyBufferPtr;
  BufferPtr                            _responseBufferPtr;

  RequestHandlerPtr                    _requestHandler;
};

typedef std::shared_ptr<Connection> ConnectionPtr;

} // namespace server
} // namespace network

