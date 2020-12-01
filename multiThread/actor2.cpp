#include <iostream>
#include <memory>
#include<string>
#include <tuple>
#include <thread>
#include <atomic>
#include <functional>
#include <boost/thread/concurrent_queues/sync_queue.hpp>

struct Message{
    enum MsgId {TASK, OK_STATUS, ER1_STATUS, ER2_STATUS};
    static void dummy(){}
    static void dummyErr(MsgId){}
    std::function<void()> _task;
    std::function<void()> _onOk;
    std::function<void(MsgId)> _onError;
    MsgId _id = TASK;
};

template< typename ... Args>  struct Actor: public std::enable_shared_from_this<Actor <Args ...>>{
    using ActorPtr = std::shared_ptr<Actor>;
    static void submit(ActorPtr from, ActorPtr to, Message msg  ){
        std::tuple< ActorPtr, Message > element(from, msg);
        to-> _queue.push(std::move(element));
    }

    static void submit(ActorPtr to, Message msg  ){
        std::tuple< ActorPtr, Message > element(ActorPtr(), msg);
        to-> _queue.push(std::move(element));
    }

    static ActorPtr make(Args&& ... args){
        return std::make_shared<Actor>(std::forward<Args>(args) ...);
    }

    virtual ~Actor() {
        _thread.join();
    }

    void stop(){
        _doneRequest = true;
        std::tuple< ActorPtr, Message > element(ActorPtr(), Message::dunmmy());
        _queue.push(std::move(element));
    }
private:
    Actor(Args&& ... args): m_args(std::forward<Args>(args) ... ){
    }
    void worker_thread() {
        while(!_doneRequest){
            auto [from, msg] = _queue.pull();
            process( std::move(from), std::move(msg));
        }
        // finishing with pending
        std::tuple< ActorPtr, Message > element(ActorPtr(), Message());
        while(boost::concurrent::queue_op_status::success == _queue.nonblocking_pull(element) ){
            auto [from, msg] = element;
            process( std::move(from), std::move(msg));
        }
    }

    void process(ActorPtr from, Message msg  ){
        if(msg._id == Message::TASK )
        {
            Message response;
            try {
                msg._task();
                response._id =  Message::OK_STATUS;
                response._onOk = std::move(msg._onOk);
                response._onError = Message::dummyErr;
            }catch(...){
                response._id =  Message::ER1_STATUS;
                response._onError = std::move(msg._onError);
                response._onOk = Message::dummy;
                // log error
            }
            if(from){
                submit(shared_from_this(), from, response );
            }
        }else if(msg._id == Message::OK_STATUS){
            msg._onOk();
        }else{
            msg._onError(msg._id);
        };
    }

private:
    std::tuple<Args ...> m_args;
    boost::concurrent::sync_queue<std::tuple< ActorPtr, Message > >         _queue;
    std::atomic<bool>                                                       _doneRequest{false};
    std::thread                                                             _thread{ [this] {worker_thread();}};
};


int main()
{
    return 0;
}



