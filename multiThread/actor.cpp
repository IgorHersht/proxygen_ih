#include <iostream>
#include <memory>
#include<string>
#include <tuple>
#include <thread>
#include <atomic>
#include <functional>
#include <boost/thread/concurrent_queues/sync_queue.hpp>


enum class MessageId {DUMMY, ERROR, MSG_ACK, DATA1, Data2,FUNC1,FUNC2 };
template<MessageId id> struct Message;


struct MessageBase {
    MessageBase(MessageId id):_id(id){}
    MessageId _id;
};
using MsgPtr = std::shared_ptr<MessageBase>;
template<> struct Message<MessageId::DUMMY> : public MessageBase{
    Message():MessageBase(MessageId::DUMMY){}
};

template<> struct Message<MessageId::ERROR> : public MessageBase{
    Message(std::string msg)
            :MessageBase(MessageId::ERROR),_msg(std::move(msg))
    {}
    std::string _msg;
};

template<> struct Message<MessageId::MSG_ACK> : public MessageBase{
    Message():MessageBase(MessageId::MSG_ACK){}
};

template<> struct Message<MessageId::DATA1> : public MessageBase{
    Message(std::string data):MessageBase(MessageId::DATA1),_data(std::move(data)) {}
    std::string _data;
};

void log(std::string msg);

enum class Actor_Id {Actor1 =1, Actor2};
template < Actor_Id> struct Actor;
struct ActorBase;
using ActorPtr = std::shared_ptr<ActorBase>;
template<Actor_Id id> ActorPtr makeActor(){
    return  std::make_shared<Actor<id>>();
}

struct ActorBase: public std::enable_shared_from_this<ActorBase>{
    void receve(MsgPtr msg, ActorPtr from) {
        _queue.push(std::make_pair(msg, from ));//move here
    }

    void receve(MsgPtr msg) {
        _queue.push(std::make_pair(msg, ActorPtr() ));//move here
    }
    void send(MsgPtr msg, ActorPtr to) {
        to->receve(msg, shared_from_this());
    }
    void stop(){
        _done = true;
        send(std::make_shared<Message<MessageId::DUMMY>>(), shared_from_this());

    }
    virtual ~ActorBase() {
        _thread.join();
    }
protected:
    virtual void process(MsgPtr msg, ActorPtr from) {

        try {
            if(!from){
                process(std::move(msg));
                return;
            }
        }catch(...){
            send(std::make_shared<Message<MessageId::ERROR>>("error"), std::move(from));
        }
    }

    virtual void process(MsgPtr msg) {

    }
private:
    void worker_thread() {
        while(!_done){
            auto item = _queue.pull();
            process( std::move(item.first), std::move(item.second));
        }
    }
private:
    boost::concurrent::sync_queue<std::pair< MsgPtr, ActorPtr >  >         _queue;
    std::atomic<bool>                                                   _done{false};
    std::thread                                                         _thread{ [this] {worker_thread();}};
};


template < > struct Actor<Actor_Id::Actor1> : public ActorBase {
    static constexpr auto id = Actor_Id::Actor1;
    void process(MsgPtr msg, ActorPtr from) override {
        ActorBase::process(msg, from);
        try {
            switch(msg->_id)
                case MessageId::DATA1 :{
                    send(std::make_shared<Message<MessageId::MSG_ACK> >(), from);
                    // ...
                }
            //...
        }catch(...){
            send(std::make_shared<Message<MessageId::ERROR>>("error"), std::move(from));
        }
    }

};

template < > struct Actor<Actor_Id::Actor2> : public ActorBase {
    static constexpr auto id = Actor_Id::Actor2;
    void process(MsgPtr msg, ActorPtr from) override {
        ActorBase::process(msg, from);
        try {
            switch(msg->_id)
                case MessageId::DATA1 :{
                    send(std::make_shared<Message<MessageId::MSG_ACK> >(), from);
                    // ...
                }
            //...
        }catch(...){
            send(std::make_shared<Message<MessageId::ERROR>>("error"), std::move(from));
        }
    }

};
///test
#include <mutex>
void log(std::string msg){
    static std::mutex m;
    std::lock_guard lck(m);
    std::cout << msg << std::endl;

}

int main()
{
    ActorPtr a1 = makeActor<Actor_Id::Actor1>();
    ActorPtr a2 = makeActor<Actor_Id::Actor2>();
    std::shared_ptr<Message<MessageId::DATA1> > m = std::make_shared< Message<MessageId::DATA1> >("m1");
    a1->send(m, a2);
    std::this_thread::sleep_for(std::chrono::seconds (5));

    a1->stop();
    a2->stop();
    int i =1;
    return 0;
}




