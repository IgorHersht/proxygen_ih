#pragma once

#include <string>
#include <vector>
#include <memory>
#include <boost/asio.hpp>

#include <common/server.h>
#include <common/request_handler.h>
#include <common/options.h>

#include "connection.h"
#include "io_context_pool.h"

namespace network {
namespace server {
struct ThrdPerCntxtServer : public Server
{

    ThrdPerCntxtServer( std::unique_ptr<common::Options> options, RequestHandlerPtr  requestHandler);
    virtual  ~ThrdPerCntxtServer();
    ThrdPerCntxtServer(const ThrdPerCntxtServer&) = delete;
    ThrdPerCntxtServer& operator =(const ThrdPerCntxtServer&) = delete;
  /// Run the ThrdPerCntxtServer's io_context loop.
  void run() override;
  void stop() override;
private:
   // void setOptions(int argc, char* argv[]);
    void setLogger();
    void setSignalHandler();
    void setHandlerThreadPool();
    void setAcceptor();

  /// Initiate an asynchronous accept operation.
  void startAccept();
  /// Handle completion of an asynchronous accept operation.
  void handleAccept(const boost::system::error_code& e);
  /// Handle a signal.
  void handleSignal(const boost::system::error_code& ec, int signalNumber);
private:
    std::unique_ptr<common::Options> _options;
  /// The pool of io_context objects used to perform asynchronous operations.
  IOContextPool _ioContextPool;
  /// The signal_set is used to register for process termination notifications.
  boost::asio::signal_set _signals;
  /// Acceptor used to listen for incoming connections.
  boost::asio::ip::tcp::acceptor _acceptor;
  /// The next Connection to be accepted.
  ConnectionPtr _newConnection;
  /// The handler for all incoming requests.
  RequestHandlerPtr  _requestHandler;

  HandlerThreadPoolPtr  _handlerThreadPool;
};

} // namespace server
} // namespace network

