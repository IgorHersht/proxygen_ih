//
// sync_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2017 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#include <string>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>
#include <common/message.h>
#include <utils/utils.h>
#include <common/logger.h>

namespace network {

struct SyncClient {

SyncClient(const std::string &host, std::string port, uint32_t maxRetry, uint32_t retryDelay) :
        _host(host), _port(port), _maxRetry(maxRetry), _retryDelay(retryDelay) {

}

void connect() {
    using boost::asio::ip::tcp;
    tcp::resolver::results_type endpoints = _resolver.resolve(_host, _port);
    boost::system::error_code ec;
    boost::asio::connect(_socket, endpoints, ec);
    if (ec) {
        log(ec.message());
        if (_retryNum < _maxRetry) {
            std::this_thread::sleep_for(std::chrono::milliseconds(_retryDelay));
            ++_retryNum;
            connect();
        } else {
            stop(ec);
        }
    }
}

void write(const AlignedMutableBuffer &requestBuffer) {
    auto buf = requestBuffer.getAsioBuffer();
    boost::system::error_code ec;
    boost::asio::write(_socket, buf, ec);// send all or error
    if (ec) {
        stop(ec);
    }
}

void read(AlignedMutableBuffer &responseHeaderBuffer,  AlignedMutableBuffer &responseBodyBuffer)
{
    readHeader(responseHeaderBuffer);
    const size_t bodySize = responseHeaderBuffer.getPtr<ResponseHeader>()->getBodySize();
    readBody(responseBodyBuffer, bodySize );
}

void stop(const boost::system::error_code &e = boost::system::error_code()) {

    if (e) {
        std::string msg = "Connection shutdown because of error ";
        msg += e.message();
        log(msg);
    } else {
        log("Connection shutdown without errors");
    }
    boost::system::error_code ignored_ec;
    _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
}

private:
    
void readHeader(AlignedMutableBuffer &buffer)
{
    buffer.setSize(sizeof(ResponseHeader));
    auto buf = buffer.getAsioBuffer();
    boost::system::error_code ec;
    boost::asio::read(_socket, buf, ec);
    if (!ec) {
        stop(ec);
    }
}

void readBody(AlignedMutableBuffer &buffer, size_t size)
{
    buffer.setSize(size);
    auto buf = buffer.getAsioBuffer();
    boost::system::error_code ec;
    boost::asio::read(_socket, buf, ec);
    if (!ec) {
        stop(ec);
    }
}
private:
    boost::asio::io_context _ioContext;
    boost::asio::ip::tcp::tcp::resolver _resolver{_ioContext};
    boost::asio::ip::tcp::tcp::socket _socket{_ioContext};
    std::string _host;
    std::string _port;
    uint32_t _retryNum{0};
    uint32_t _maxRetry;
    uint32_t _retryDelay;// milliseconds
};

}//namespace network
// test
#include <iostream>
#include <ostream>

int main(int argc, char* argv[])
{
  try
  {
      network::SyncClient client("localhost","8080", 10, 1000);
      client.connect();
/////////
      std::string requestBody = "client request body";
      network::RequestHeader::msg_type msgType = 7;
      network::RequestHeader requestHeader(requestBody.size(), msgType);

      network::AlignedMutableBuffer requestHeaderBuffer;
      requestHeaderBuffer.setBuffer(&requestHeader, sizeof(network::RequestHeader) );
      network::AlignedMutableBuffer   requestBodyBuffer;
      requestBodyBuffer.setBuffer(requestBody.c_str(), requestBody.size() );

      network::AlignedMutableBuffer sendBuffer = requestHeaderBuffer + requestBodyBuffer;

      client.write(sendBuffer);
  //////////////////////////////////////////
      network::AlignedMutableBuffer responseHeaderBuffer, responseBodyBuffer;
      client.read(responseHeaderBuffer, responseBodyBuffer);
  ////
      client.stop();
 //////
 // test
      network::ResponseHeader* header = responseHeaderBuffer.getPtr<network::ResponseHeader>();
      size_t s = responseBodyBuffer.getSize();
      char* t2= new char[s];
      memcpy(t2, responseBodyBuffer.getPtr<char>(), s);
      int i =1;
  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
