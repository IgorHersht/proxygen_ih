#pragma once

#include <list>
#include <vector>
#include <memory>
#include <boost/asio.hpp>

namespace network {
namespace server {

/// A pool of io_context objects.
struct  IOContextPool
{
  /// Construct the io_context pool.
  IOContextPool() :_nextIOContext(0){

  }
  explicit IOContextPool(std::size_t poolSize);
  IOContextPool(const IOContextPool&) = delete;
  IOContextPool& operator =(const IOContextPool&) = delete;
  /// Run all io_context objects in the pool.
  void set(std::size_t poolSize);
  void run();
  /// Stop all io_context objects in the pool.
  void stop();
  /// Get an io_context to use.
  boost::asio::io_context& getIOContext();
private:
  typedef std::shared_ptr<boost::asio::io_context> IOContextPtr;
  typedef boost::asio::executor_work_guard< boost::asio::io_context::executor_type> IOContextWork;
  /// The pool of io_contexts.
  std::vector<IOContextPtr> _ioContexts;
  /// The work that keeps the io_contexts running.
  std::list<IOContextWork> _work;
  /// The next io_context to use for a Connection.
  std::size_t _nextIOContext;
};

} // namespace ThrdPerCntxtServer
} // namespace network


