
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>
#include <iostream>
#include <exception>
#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<list>
#include<vector>
#include<queue>
#include<algorithm>
#include "boost/multi_array.hpp"

 template <size_t... Entries>
struct StaticArray
{
  enum {N = sizeof...(Entries)};
  size_t array[N]{Entries...};
};

template <typename T>
void output(T&& t)
{
  std::cout << t;
}
 
template <typename First, typename... Args>
void output(First&& f, Args&&... args)
{
  std::cout << f << " ";
  output(std::forward<Args>(args)...);
}

int main(int argc, char* argv[])
{

    output( 5, "XXXXX", 4.8/2.1 );
    StaticArray<1, 5, 8> s1;
  
    
 return 0;
}
 
