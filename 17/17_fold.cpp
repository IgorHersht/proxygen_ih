#include <queue>
#include <atomic>
#include <vector>
#include <list>
#include <deque>
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

#include <type_traits>
#include <stdlib.h>


template<typename ... Args> auto foldSum(Args ... args){
    return (... + args);
}
//
template <typename Arg> void foo( const  Arg& arg){
    std::cout << arg << std::endl;
}
template <typename ... Args> void callFoos( Args&& ... args){
    (... , foo(std::forward<Args>(args) ) );
}

template <typename T1, typename ...TN> constexpr bool isTheSame(T1 , TN ...){
    return(std::is_same_v<T1, TN> && ...);

}
int main()
{
    std::cout << "foldSum="<< foldSum(1.2 + 2) << std::endl;
    callFoos(1, 5.5, "OK");
    constexpr bool sm = isTheSame(2, 5);
    return 0;
}

