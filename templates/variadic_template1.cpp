
#include <boost/optional/optional.hpp>
#include "boost/utility/in_place_factory.hpp"
#include "boost/utility/typed_in_place_factory.hpp"
#include <iostream>
#include <tuple>


template <typename T> T p1(const T &t)
{
  return t +1;
}

template <typename T> void print(const T &t)
{
  std::cout << t<< ":"<< std::flush;
}

template <typename T, typename ... Args> void print(const T &t, const Args& ... args)
{
  print(t);
  print(args...);
}

template <typename ... Args> void printp1(const Args& ... args)
{

  print(p1(args)...);
}

template <typename ... Args> struct S
{

  typedef std::tuple<Args ...> args_type;
  static void print()
  {
    std::cout<< "Args num=" << sizeof...(Args)<<std::endl;


  }
};




int main()
{


  print(4, 5.6);
  std::cout<<std::endl;
  printp1(4, 5.6);
  S<int, double, std::string>::print();
   return 0;
}
