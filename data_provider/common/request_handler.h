#pragma once
#include <memory>
#include "message.h"


namespace network
{
struct RequestHandler
{
    virtual void handleRequest(BufferPtr requestHeaderBufferPtr, BufferPtr requestBodyBuffer, BufferPtr& responseBuffer ) =0 ;
};

typedef std::shared_ptr<RequestHandler> RequestHandlerPtr;



}//namespace network