 
#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <string>
#include <boost/thread/thread.hpp>
#include <boost/thread/once.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <iostream>
#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <functional>
#include <numeric>
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include <iostream>
#include <typeinfo>
#include <algorithm>
#include <iterator>
#include <memory>
#include <string>
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <thread>

#include <memory>
#ifndef _MAKE_UNIQUE_H
#define  _MAKE_UNIQUE_H

namespace tools
{

 template <class T, typename ...Args>  
 inline std::unique_ptr<T> make_unique(Args&& ...args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
}
#endif

template <class T> struct Deleter{
    void operator()(T *p){
        delete p;
    }
};


template <class T> struct Deleter<T[]>{
    void operator()(T *p){
        delete []p;
    }
};

std::unique_ptr<int[]> p{new int[3]};

int main(int, char*[])
{
    std::unique_ptr<std::string> pu= tools::make_unique<std::string>(std::string("XXXXXXX"));
    std::cout <<sizeof(std::unique_ptr<std::string>::pointer) <<std::endl;
    std::cout <<sizeof(std::unique_ptr<std::string>::deleter_type) <<std::endl;
    std::cout <<sizeof(std::unique_ptr<std::string>) <<std::endl;
    std::cout <<sizeof(std::tuple<std::unique_ptr<std::string>::pointer, std::unique_ptr<std::string>::deleter_type>) <<std::endl;
    
     std::unique_ptr<std::string, Deleter<std::string> > p2(new std::string("yyyyyy"));
     std::unique_ptr<std::string[], Deleter<std::string[]> > pv(new std::string[10]{"yyyyyy", "yyyyyy"});
    
    

    return 0;
}
