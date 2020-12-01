#include <map>

#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>
#include <numeric>
#include <atomic>

struct  C{
    template< typename T> void foo(T t){
        std::cout << "template< typename T> foo(T t)" <<std::endl;
    }
}; 

 template< > void C::foo<int>(int t){
        std::cout << "template< > C::foo<int>(int t)" <<std::endl;
    }
 
int main()
{
    
    C c1;
    c1.foo("c");
      c1.foo(5);
  

 
    
    return 0;
}
