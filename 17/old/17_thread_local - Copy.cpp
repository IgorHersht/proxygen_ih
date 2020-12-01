#include <queue>
#include <atomic>
#include <vector>
#include <list>
#include <future>
#include <algorithm>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <boost/circular_buffer.hpp>
#include<iostream>
#include<string>
#include<atomic>
#include <sstream>

#include <memory>
#include <stdlib.h>




//std::thread::id this_id = std::this_thread::get_id()
using Threads = std::vector< std::future<void> > ;
inline thread_local  std::string local;
Threads threads;

struct ThreadEntry{

    ThreadEntry(int index):
            m_index(index){}
    void operator()(){
        std::stringstream ss;
        ss << std::this_thread::get_id();
        local= ss.str();// write to local
        std::string data = "theard number=";
        data   += std::to_string(m_index) + " has id= " + local; // read local
        int j =1;
    }

    int m_index;
};



int main()
{

    for( int i =0; i <5; ++i){
        ThreadEntry entry(i);
        std::future<void> f = std::async( std::launch::async, std::move(entry));
        threads.emplace_back(std::move(f));
    }
    for(auto & f: threads){
        f.get();
    }


    return 0;
}

