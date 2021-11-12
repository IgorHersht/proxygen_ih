#include "server_options.h"
#include "thrd_per_cntxt_server.h"

namespace network {
namespace server {

ThrdPerCntxtServer::ThrdPerCntxtServer( std::unique_ptr<common::Options> options, RequestHandlerPtr  requestHandler):
    _options(std::move(options)),
    _ioContextPool(*(_options->get<int>("net.server.io_threads"))),
    _signals(_ioContextPool.getIOContext()),
    _acceptor(_ioContextPool.getIOContext()),
    _newConnection(),
    _requestHandler(requestHandler)
{
    setLogger();
    setSignalHandler();
    setHandlerThreadPool();
    setAcceptor();
    startAccept();
}

ThrdPerCntxtServer::~ThrdPerCntxtServer()
{
    try{
        stop();
    }catch(...){

    }
}

// TODO
void ThrdPerCntxtServer::setLogger()
{
}

void ThrdPerCntxtServer::setSignalHandler()
{
    // Register to handle the signals that indicate when the ThrdPerCntxtServer should exit.
    // It is safe to register for the same signal multiple times in a program,
    // provided all registration for the specified signal is made through Asio.
    _signals.add(SIGINT);
    _signals.add(SIGTERM);
#if defined(SIGQUIT)
    _signals.add(SIGQUIT);
#endif // defined(SIGQUIT)
    _signals.async_wait([this](const boost::system::error_code& ec, int signalNumber){ handleSignal(ec, signalNumber);});
}
//
void ThrdPerCntxtServer::setHandlerThreadPool()
{
    int thrNum = *(_options->get<int>("net.server.cpu_threads"));
    if(thrNum > 0)
    {
        _handlerThreadPool = HandlerThreadPoolPtr(new boost::asio::thread_pool(thrNum), [](boost::asio::thread_pool* ptr){
                ptr->join();
                ptr->stop();
                delete ptr;
            } );
    }
}
void ThrdPerCntxtServer::setAcceptor()
{
    const std::string address = *(_options->get<std::string>("net.server.address"));
    const std::string port  = *(_options->get<std::string>("net.server.port"));
    boost::asio::ip::tcp::resolver resolver(_acceptor.get_executor().context());
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(address, port).begin();
    _acceptor.open(endpoint.protocol());
    _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    _acceptor.bind(endpoint);
    _acceptor.listen();
}
void ThrdPerCntxtServer::run()
{
  _ioContextPool.run();
}

void ThrdPerCntxtServer::stop()
{
    try{
        _ioContextPool.stop();
    }catch(...){

    }
}

void ThrdPerCntxtServer::startAccept()
{
  _newConnection.reset(new Connection(_ioContextPool.getIOContext(),_handlerThreadPool, _requestHandler));
  _acceptor.async_accept(_newConnection->socket(), [this](const boost::system::error_code& e){ handleAccept(e);} );
}

void ThrdPerCntxtServer::handleAccept(const boost::system::error_code& e)
{
  if (!e)
  {
    _newConnection->start();
  }
    startAccept();
}

void ThrdPerCntxtServer::handleSignal(const boost::system::error_code &ec, int signalNumber)
{
  stop();
}

} // namespace ThrdPerCntxtServer
} // namespace network
