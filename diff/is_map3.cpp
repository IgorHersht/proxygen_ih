#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <forward_list>
#include <iostream>
#include <boost/tti/tti.hpp>

template< typename T, typename ... Types> struct IsMap{
 static constexpr bool value = false;
};

template< typename ... Types> struct IsMap<std::map<Types ...>>{
  static constexpr bool value = true;
};

template< typename ... Types> struct IsMap<std::unordered_map<Types ...>>{
  static constexpr bool value = true;
};


int main(){

  std::set<int> s;  std::map<int,int> m;  std::unordered_map<int,int> um;

  bool b1 =  IsMap<decltype(s)>::value;
  bool b2 =  IsMap<decltype(m)>::value;
  bool b3 =  IsMap<decltype(um)>::value;


  return 0;
  }
