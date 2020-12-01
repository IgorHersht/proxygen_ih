
#include <thread>
#include <iostream>
#include <memory>
#include<string>
#include <tuple>
#include<queue>
#include <variant>
#include <type_traits>

template < template<typename> typename QT, typename ... TS   > struct VariantQueue{
    using ElementT = std::variant< TS ... >;
    using QueueT =QT< ElementT>;
    using QueuePtr =std::unique_ptr<QT< ElementT>>;
    VariantQueue() = default;
    VariantQueue(QueuePtr&& queue):_queue(std::move(queue)){
    }
    template <typename T> void push(T&& t){
        ElementT el(std::forward<T>(t)) ;
        _queue->push(std::move(el));
    }
    ElementT pop(){
        return _queue->pull();
    }
private:
//    template <typename ... Args> struct Visitor{
//        Visitor(Args&& ... args):_args(std::forward<Args>(args) ... ){
//        }
//
//        template <typename T> void operator()( T & inv) const{
//            //std::apply(std::forward<F>(_f), std::forward<decltype(_args)>(_args) );
//        }
//        
//        std::tuple<Args ...> _args;
//    };
private:
    QueuePtr  _queue;
};



/// test
#include <boost/thread/concurrent_queues/sync_queue.hpp>
using Queue =  VariantQueue<   boost::sync_queue, double, int >;
Queue  q ( std::make_unique<Queue::QueueT >());



int main(){
    q.push(24);
    auto v(q.pop());
    int i = 0;

    return 0;
}

