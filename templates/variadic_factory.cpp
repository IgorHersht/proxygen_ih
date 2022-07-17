

#include <typeinfo>
#include <string>
#include <array>
#include <initializer_list>
#include <queue>
#include <vector>
#include <list>
#include <deque>
#include <new>
#include <tuple>

#include <thread>
#include <memory>
#include <mutex>
#include <future>
#include <memory>
#include <functional>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm>


struct B1 {
  B1(int v) {}
};

struct B2 {
  B2(int, std::string){} 
};


template <class T, class ...Args>  // create a object T, with argumets Args ...params
    std::shared_ptr<T>  make (Args ...params){
    int n2 = sizeof... (Args); // argument number (compile time)
    typedef std::tuple< Args ... > ArgsTupleT;
    enum {ArgsNum = std::tuple_size< ArgsTupleT > ::value }; // argument number (compile time)
    int n = ArgsNum;
    typedef typename std::tuple_element<0, ArgsTupleT > ::type FirstArgT;// First argument type
    return std::shared_ptr<T>(new T( params... ));
  }


int main()
{
  auto ap = make<B1>(25);// creates B1
  auto b1p =make<B2>(25, "xxx"); // creates B2

  return 0;
}
