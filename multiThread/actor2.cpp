#include <memory>
#include<string>
#include <tuple>
#include <thread>
#include <atomic>
#include <functional>
#include <boost/thread/concurrent_queues/sync_queue.hpp>

void log(std::string msg);
// TODO split into diff messages
struct Message{
    enum MsgId {TASK, OK_STATUS, ER1_STATUS, ER2_STATUS};
    static void dummy(){}
    static void dummyErr(MsgId){}
    std::function<void()> _task;
    std::function<void()> _onOK;
    std::function<void()> _onOkResponse;
    std::function<void(MsgId)> _onErrorResponse;
    MsgId _id = TASK;
};

template<typename Id, typename ... Args>  struct Actor: public std::enable_shared_from_this<Actor <Id, Args ...>>{
    using ActorPtr = std::shared_ptr<Actor>;

    static ActorPtr make( Id&& id, Args&& ... args){
        return ActorPtr(new Actor(std::forward<Id>(id), std::forward<Args>(args) ...));
    }
    friend void submit(ActorPtr from, ActorPtr to, Message msg  ){
        from->submit(std::move(to), std::move(msg));
    }

    friend void submit(ActorPtr to, Message msg  ){
        std::tuple< ActorPtr, Message > element(ActorPtr(), msg);
        to-> _queue.push(std::move(element));
    }

    virtual ~Actor() {
        _thread.join();
    }

    void stop(){
        _doneRequest = true;
        std::tuple< ActorPtr, Message > element(ActorPtr(), Message::dummy());
        _queue.push(std::move(element));
    }

private:
    Actor(Id&& id, Args&& ... args):
    _id(std::forward<Id>(id)  ),
    m_args(std::forward<Args>(args) ... ){
    }
    void submit(ActorPtr to, Message msg  ){
        std::tuple< ActorPtr, Message > element(this->shared_from_this(), msg);
        to-> _queue.push(std::move(element));
    }
    void worker_thread() {
        while(!_doneRequest){
            auto [from, msg] = _queue.pull();
            process( std::move(from), std::move(msg));
        }
        // finishing with pending
        std::tuple< ActorPtr, Message > element = std::make_tuple(ActorPtr(), Message());
        while(boost::concurrent::queue_op_status::success == _queue.try_pull(element) ){//TODO deal with other statuses
            process( std::move(std::get<0>(element)), std::move(std::get<1>(element)));
        }
    }

    virtual void process(ActorPtr from, Message msg  )
    {
        try {
            if (msg._id == Message::TASK) {
                Message response;
                try {
                    msg._task();
                    response._id = Message::OK_STATUS;
                    response._onOkResponse = std::move(msg._onOkResponse);
                    msg._onOK();
                } catch (...) {// TODO catch diff exceptions
                    response._id = Message::ER1_STATUS;
                    response._onErrorResponse = std::move(msg._onErrorResponse);
                    // log error
                }
                if (from) {
                    submit(from, response);
                }

            } else if (msg._id == Message::OK_STATUS) {
                msg._onOkResponse();
            } else {
                msg._onErrorResponse(msg._id);
            };
        }catch(...){
            // log error
        }
    }

private:
    Id                                                                      _id;
    std::tuple<Args ...>                                                    m_args;
    boost::concurrent::sync_queue<std::tuple< ActorPtr, Message > >         _queue;
    std::atomic<bool>                                                       _doneRequest{false};
    std::thread                                                             _thread{ [this] {worker_thread();}};
};



///test
#include <mutex>
void log(std::string msg){
    static std::mutex m;
    std::lock_guard lck(m);
    std::cout << msg << std::endl;

}

using Actor1 = Actor<std::string>;

auto a1 = Actor1::make("Actor1");
auto a2 = Actor1::make("Actor2");

void task(){
    log("submitets from main therad to Actor1");


}


int main()
{
   Message mgs;


    return 0;
}



