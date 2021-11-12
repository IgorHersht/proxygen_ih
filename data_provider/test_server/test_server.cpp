
#include <iostream>
#include <memory>
#include "common/request_handler.h"
#include "server/thrd_per_cntxt_server.h"
#include "server/server_options.h"


struct EchoRequestHandler : public network::RequestHandler
{
//      void handleRequest(const network::AlignedMutableBuffer& requestHeaderBuffer, const network::AlignedMutableBuffer& requestBodyBuffer,
//                         network::AlignedMutableBuffer& responseHeaderBuffer, network::AlignedMutableBuffer& responseBodyBuffer) override
//      {
//          responseHeaderBuffer = requestHeaderBuffer;
//          responseBodyBuffer = requestBodyBuffer;
//      }

   void handleRequest(network::BufferPtr requestHeaderBufferPtr, network::BufferPtr requestBodyBuffer, network::BufferPtr& responseBuffer ) override
   {
       responseBuffer = requestHeaderBufferPtr + requestBodyBuffer;
   }

};

//struct EchoRequestHandler2 : public network::RequestHandler2
//{
//    void makeResponse() override
//    {
//       _responseBuffer = _requestHeaderBufferPtr + _requestBodyBuffer ;
//    }
//
//};

int main(int argc, char* argv[])
{
  try
  {

  std::unique_ptr<common::Options> options (new network::server::ServerOptions);
  options->set(argc, argv);
  options->validate();

  network::RequestHandlerPtr handler= std::make_shared<EchoRequestHandler>();
  network::ServerPtr server = std::make_shared< network::server::ThrdPerCntxtServer>(std::move(options), handler );
  server->run();


  }catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
    return 2;
  }
  catch (...)
  {
      std::cerr << "unknown exception: " << "\n";
      return 3;
  }

  return 0;
}
