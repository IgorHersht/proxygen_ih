#include <iostream>
#include <vector>
#include<string>
#include <tuple>
#include <thread>
#include <boost/lockfree/spsc_queue.hpp>

using DP = std::pair<std::thread, boost::lockfree::spsc_queue<int, boost::lockfree::capacity<1024> > >;
std::vector<DP> dps(5) ;

void push1(int i,DP &dp ){
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    bool status = dp.second.push(i);
        if(!status){
            std::this_thread::yield();
        }
}

void runThread(DP* dpp){
   for(int i = 0; i < 10; ++i){
       push1(i, *dpp) ;
   }
}

void runThreads() {
    for (auto &dp: dps) {
        std::thread t( [dpp = &dp]{runThread(dpp);} );
        dp.first = std::move(t);
    }
}

void popAll(){
    while(true){
        for (auto &dp: dps) {
            int i = -1;
            bool status = dp.second.pop(i);
            if(status){
                std::cout << i << std::endl;
            }
        }
    }
}

int main()
{
    runThreads();
    std::thread t(popAll);

    for (auto &dp: dps) {
       dp.first.join();
    }
    t.join();

    return 0;
}


