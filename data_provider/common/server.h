#pragma once

#include <memory>

namespace network
{
struct Server
{
    virtual   void run() = 0;
    virtual void stop() = 0;
    virtual ~Server(){}
};
    typedef std::shared_ptr<Server> ServerPtr;


}// namespace network