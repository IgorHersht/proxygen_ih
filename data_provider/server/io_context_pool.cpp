#include <stdexcept>
#include "thrd_per_cntxt_server.h"

namespace network {
namespace server {

IOContextPool::IOContextPool(std::size_t poolSize) : _nextIOContext(0)
{
  if (poolSize == 0)
    throw std::runtime_error("IOContextPool size is 0");

  // Give all the io_contexts work to do so that their run() functions will not
  // exit until they are explicitly stopped.
  for (std::size_t i = 0; i < poolSize; ++i)
  {
    IOContextPtr io_context(new boost::asio::io_context);
    _ioContexts.push_back(io_context);
    _work.push_back(boost::asio::make_work_guard(*io_context));
  }
}

void IOContextPool::set(std::size_t poolSize)
{
    if (poolSize == 0)
        throw std::runtime_error("IOContextPool size is 0");

    // Give all the io_contexts work to do so that their run() functions will not
    // exit until they are explicitly stopped.
    for (std::size_t i = 0; i < poolSize; ++i)
    {
        IOContextPtr io_context(new boost::asio::io_context);
        _ioContexts.push_back(io_context);
        _work.push_back(boost::asio::make_work_guard(*io_context));
    }
}

void IOContextPool::run()
{
  // Create a pool of threads to run all of the io_contexts.
  std::vector<std::shared_ptr<std::thread> > threads;
  for (std::size_t i = 0; i < _ioContexts.size(); ++i)
  {
    std::shared_ptr<std::thread> thread(new std::thread([this, i] { _ioContexts[i]->run(); }));
    threads.push_back(thread);
  }
  // Wait for all threads in the pool to exit.
  for (std::size_t i = 0; i < threads.size(); ++i)
    threads[i]->join();
}

void IOContextPool::stop()
{
  // Explicitly stop all io_contexts.
  for (std::size_t i = 0; i < _ioContexts.size(); ++i)
    _ioContexts[i]->stop();
}

boost::asio::io_context& IOContextPool::getIOContext()
{
  // Use a round-robin scheme to choose the next io_context to use.
  boost::asio::io_context& io_context = *_ioContexts[_nextIOContext];
  ++_nextIOContext;
  if (_nextIOContext == _ioContexts.size())
      _nextIOContext = 0;
  return io_context;
}

} // namespace ThrdPerCntxtServer
} // namespace network
