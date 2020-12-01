#include <boost/assert.hpp>
#include <map>
#include <string>
#include <memory>
#include <iostream>
#include <thread>
#include <mutex>
#include <wangle/concurrent/CPUThreadPoolExecutor.h>
#include <boost/thread/executors/basic_thread_pool.hpp>
#include <boost/thread/future.hpp>


struct ThreadContext{

};


static std::vector<std::thread::id> threadIds;

template <typename TCT> struct ThreadPoolWithContext{
typedef std::map<std::thread::id, std::unique_ptr<TCT> > MapT;


template <typename... Args > ThreadPoolWithContext(Args&&... args ): m_thread_pool(4, at_th_entry ){
	for(auto &id: threadIds){
		 std::unique_ptr<TCT> ptr = std::make_unique<TCT>( std::forward<Args>(args)...);
		m_contexts.insert(std::pair<std::thread::id, std::unique_ptr<TCT> >(id, std::move(ptr) ) );
	}

};
private:

static void at_th_entry(boost::basic_thread_pool& pool){
	threadIds.push_back(std::this_thread::get_id());
}
private:
	boost::basic_thread_pool							m_thread_pool;
	std::map<std::thread::id, std::unique_ptr<TCT> >	m_contexts;

};

int main(int argc, char* argv[])
{
	ThreadPoolWithContext<ThreadContext> context;


	return 0;
}

