#include <vector>
#include <atomic>
#include <boost/lockfree/spsc_queue.hpp>


template<typename T, size_t producerCapacity> class mpsc_queue {
    using ProducerQueue = boost::lockfree::spsc_queue<int, boost::lockfree::capacity<producerCapacity> > ;// ProducerQueue
    using ProducerQueues = std::vector<ProducerQueue>;
public:
    using ConsumerBuffer = std::vector<T>;
    explicit mpsc_queue(size_t produsersNum):
            _producerQueues(produsersNum){
        _consumerBuf.reserve(produsersNum * producerCapacity );
    }

    template<typename ConstIterator> bool  produce(ConstIterator begin, ConstIterator end,  size_t  produserNum)
    {
        while((!_stop) && (begin != end)){
            begin = _producerQueues[produserNum].push(begin, end);
        }
        if(_stop){
            ++_donePorodicers;
        }
        return _stop;
    }

    template<typename F> void consume(F&& f)
    {

        while(true)
        {
            for(ProducerQueue& queue: _producerQueues )
            {
                consume(queue) ;
            }
            std::forward<F>(f)(std::move(_consumerBuf));
            _consumerBufOffset = 0;
            if(_stop && (_donePorodicers == _producerQueues.size())){
                break;
            }
        }
    }

    void stop(){
        _stop = true;
    }
    bool isStopped() const{
        return _stop;
    }

private:
    void consume(ProducerQueue& queue)
    {
         std::array<T,producerCapacity> buf;
        while(true)
        {
            size_t offset = queue.pop(&(*buf.begin()), buf.size());
            if(!offset){
                break;
            }

            _consumerBuf.insert(_consumerBuf.begin() + _consumerBufOffset , buf.begin(),  buf.begin() + offset );
            _consumerBufOffset += offset;
        }
    }

private:
    ProducerQueues          _producerQueues;
    ConsumerBuffer          _consumerBuf;
    size_t                  _consumerBufOffset{0};
    std::atomic<bool>       _stop{false};
    std::atomic<size_t>     _donePorodicers{0};
};
//test
#include <iostream>
#include <thread>
#include <future>
#include <chrono>

constexpr size_t producerCapacity =  1024;
constexpr size_t produsersNum =  16;
using Queue = mpsc_queue<int, producerCapacity>;
Queue queue(produsersNum);


void produceOne(size_t prNum)
{
        bool done = false;
        while(!done)
        {
            // emulate load
            std::vector<size_t>  v;
            for( size_t i = 1; i < producerCapacity/2; ++i){
                v.emplace_back(i);
            }
            done = queue.produce(v.cbegin(), v.cend(), prNum  );
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
}

void f(Queue::ConsumerBuffer buf){
    std::cout<< "receved size="<< buf.size() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(50 ));
    buf.clear();
}
void consume()
{
        queue.consume(f);
}

int main(){
    std::vector<std::future<void>> fs;
    for(size_t i = 0; i <produsersNum; ++i){
        fs.emplace_back(std::async(std::launch::async,[i]{
                produceOne(i);
        }));
    }
    std::future<void> consumer = std::async(std::launch::async,consume);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    queue.stop();


  try{
      for(size_t i = 0; i <produsersNum; ++i){
          fs[i].wait();
      }
  }catch(...){
    int i =1;
  }
    try {
        consumer.wait();
    }catch(...){
        int i =1;
    }

    int i =1;
    return 0;
}


