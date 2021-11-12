#include <vector>
#include <common/message.h>
#include <utils/utils.h>
#include <common/logger.h>
#include "connection.h"

#define _DEBUG
namespace network {
namespace server {

 struct RequestHandlerCallback
 {
     RequestHandlerCallback(std::shared_ptr<Connection> connection, BufferPtr requestHeaderBufferPtr, BufferPtr requestBodyBufferPtr)
     :_connection(connection),
      _requestHeaderBufferPtr(std::move(requestHeaderBufferPtr)),
      _requestBodyBufferPtr(std::move(requestBodyBufferPtr))
     {
     }
     void operator()(){
         // in thread_pool thread
         _connection->_requestHandler->handleRequest(
                 std::move(_requestHeaderBufferPtr),std::move(_requestBodyBufferPtr), _responseBufferPtr);
         std::shared_ptr<Connection> connection = _connection;
         BufferPtr  responseBufferPtr(std::move(_responseBufferPtr));
         // post back the connection thread
         boost::asio::post(_connection->_socket.get_io_context(),[connection, responseBufferPtr]{
             // in io thread
             connection->_responseBufferPtr = std::move(responseBufferPtr);
             connection->onRequestHandled() ;
         });
     }
 private:
     std::shared_ptr<Connection> _connection;
     BufferPtr _requestHeaderBufferPtr;
     BufferPtr _requestBodyBufferPtr;
     BufferPtr  _responseBufferPtr;
 };

Connection:: Connection(boost::asio::io_context& ioContext, HandlerThreadPoolPtr handlerThreadPool, RequestHandlerPtr requestHandler)
  : _socket(ioContext),
    _handlerThreadPool(handlerThreadPool),
    _requestHandler(requestHandler)
{
}

boost::asio::ip::tcp::socket& Connection::socket(){
  return _socket;
}

void Connection::stop(const boost::system::error_code& e)
{
    if(e){
        std::string msg = "Connection shutdown because of error ";  msg += e.message();
        log(msg);
    }else{
        log("Connection shutdown without errors");
    }
    boost::system::error_code ignored_ec;
    _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
}

void Connection::start()
{
    readHeader();
}
void Connection::readHeader()
{
    _requestHeaderBufferPtr = std::make_shared<AlignedMutableBuffer>();
    _requestHeaderBufferPtr->setSize(sizeof(RequestHeader));
    auto buf = _requestHeaderBufferPtr->getAsioBuffer();
    auto self(shared_from_this());
    async_read(_socket, buf, [this, self](const boost::system::error_code& ec, std::size_t length) {
        if(!ec){
            handleReadHeader(ec, length);
        }else{
            stop(ec);
        }
    });
}
void Connection::handleReadHeader(const boost::system::error_code& e, std::size_t bytesTransferred)
{
    if(e){
        stop(e);
        return;
    }
    if(bytesTransferred != _requestHeaderBufferPtr->getSize()){
        boost::system::error_code e(boost::asio::error::message_size);
        stop(e);
        return;
    }

    readBody();
}

void Connection::readBody()
{
    const size_t bodySize = _requestHeaderBufferPtr->getPtr<RequestHeader>()->getBodySize();
    if(!bodySize){
        boost::system::error_code e(boost::asio::error::message_size);
        stop(e);
        return;
    }

    _requestBodyBufferPtr = std::make_shared<AlignedMutableBuffer>();
    _requestBodyBufferPtr->setSize(bodySize);
    auto buf = _requestBodyBufferPtr->getAsioBuffer();
    auto self(shared_from_this());
    async_read(_socket, buf, [this, self](const boost::system::error_code& ec, std::size_t length) {
        if(!ec){
            handleReadBody(ec, length);
        }else{
            stop(ec);
        }
    });

}


void Connection::handleReadBody(const boost::system::error_code& e, std::size_t bytesTransferred)
{
    if(e){
        stop(e);
        return;
    }
    if(bytesTransferred != _requestBodyBufferPtr->getSize()){
        boost::system::error_code e(boost::asio::error::message_size);
        stop(e);
        return;
    }
#ifdef _DEBUG
    char* t1= new char[bytesTransferred];
    memcpy(t1, _requestBodyBufferPtr->getPtr<void>(), bytesTransferred);
    int i1 =1;
#endif
    if(!_handlerThreadPool){// no thread pool =>blocking call!!
        _requestHandler->handleRequest(std::move(_requestHeaderBufferPtr), std::move(_requestBodyBufferPtr), _responseBufferPtr);
        onRequestHandled();
    }
    else// submit to the thread poll
    {
        auto self(shared_from_this());
        RequestHandlerCallback callback(self, std::move(_requestHeaderBufferPtr), std::move(_requestBodyBufferPtr) );
        boost::asio::post(*_handlerThreadPool, callback);
    }
}

void Connection::onRequestHandled()
{
#ifdef _DEBUG
    size_t s = _responseBufferPtr->getSize();
    const ResponseHeader* headerPtr =  _responseBufferPtr->getPtr<ResponseHeader>();
    char* t2= new char[s - sizeof(ResponseHeader)];
    memcpy(t2, _responseBufferPtr->getPtr<char>() + sizeof(ResponseHeader), s - sizeof(ResponseHeader) );
    int i2 =1;
#endif
    write();
}

void Connection::write()
{
    auto buf = _responseBufferPtr->getAsioBuffer();
    auto self(shared_from_this());
    boost::asio::async_write(_socket, buf, [this, self](boost::system::error_code ec, std::size_t length)
         {
             if(!ec){
                 handleWrite(ec, length);
             }else{
                 stop(ec);
             }
         });
}

void Connection:: handleWrite(const boost::system::error_code& e, std::size_t bytesTransferred)
{
    if(e){
        stop(e);
        return;
    }
    if(bytesTransferred != _responseBufferPtr->getSize()){
        boost::system::error_code e(boost::asio::error::message_size);
        stop(e);
        return;
    }

    readHeader();
}

} // namespace server
} // namespace network
