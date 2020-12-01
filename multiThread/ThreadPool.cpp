
#include <thread>
#include <vector>
#include <atomic>
#include <functional>
#include <boost/thread/sync_bounded_queue.hpp>

struct ThreadPool {
	typedef std::function<void()> FuncT;

    ThreadPool(size_t queue_size, size_t thread_count = std::thread::hardware_concurrency() ):m_done(false), m_work_queue(queue_size){

        try{
            for(size_t i=0;i<thread_count;++i){
                m_threads.push_back(
                    std::thread(&ThreadPool::worker_thread,this));
            }
        } catch(...){
            m_done=true;
            join();
        }
    }

    ~ThreadPool(){
        m_done=true;
        join();
    }
    void submit(FuncT f){
        m_work_queue.push(f);
    }
private:

   void worker_thread() {
	   while(!m_done){
		   FuncT task;
		   if(m_work_queue.try_pull(task)){
			   task();
		   }else{
			   std::this_thread::yield();
		   }
	   }
   }

void join(){
	for(auto & th: m_threads){
		try{
			if(th.joinable()){
				th.join();
			}
		}catch(...){

		}
	}
}
private:
   std::atomic_bool 												m_done;
   boost::concurrent::sync_bounded_queue<FuncT > 					m_work_queue;
   std::vector<std::thread> 										m_threads;

};
// test
#include <chrono>
void foo(){
	std::this_thread::sleep_for(std::chrono::seconds(2));
}

int main()
{
	ThreadPool p(5);
	p.submit(foo);
	std::this_thread::sleep_for(std::chrono::seconds(200));// just for test - make sure that main thread is here

	return 0;
}


