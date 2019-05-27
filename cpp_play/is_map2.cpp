

#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <iostream>
#include <boost/tti/tti.hpp>



template <typename Container> decltype(auto) getKeys_impl(const Container &c, std::true_type ){ // map

  std::set< typename Container::key_type> s;
  for( const auto & el: c){
    s.emplace(el.first);
  }
  return s;
}

template <typename Container> decltype(auto) getKeys_impl(const Container &c, std::false_type ){ // not map

  std::set< typename Container::value_type> s;
  for( const auto & el: c){
    s.emplace(el);
  }
  return s;
}

BOOST_TTI_HAS_TYPE(mapped_type)
  template <typename Container> decltype(auto) getKeys(const Container &c ){
  constexpr bool v = has_type_mapped_type< Container >::value;
  return getKeys_impl(c, std::integral_constant<bool, v>() );
}

// test
std::vector<int> v {1,2};
std::list<int> l {3,4};
std::set<int> s{5,6};
std::map<int,int> m{{11,1}, {12,1}};
std::multimap<int,int>mm{ {21,1}, {22,1}};
std::unordered_map<int,int> um{ {31,1}, {32,1}};


int main(){


    std::set<int> s1 = getKeys(v );
    std::set<int> s2 =  getKeys(l );
    std::set<int> s3 = getKeys(s);

  std::set<int> s11 = getKeys(m );
  std::set<int> s12 =  getKeys(mm );
  std::set<int> s13 = getKeys(um);

  return 0;
  }
