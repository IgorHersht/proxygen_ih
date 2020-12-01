
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



///////////////
class Widget{};
Widget w;
bool f(const Widget& w);// decltype(f) is bool(const Widget&)
                        // decltype(f(w)) is bool
///////////
template<typename Container, typename Index>
auto authAndAccess(Container& c, Index i) -> decltype(c[i]) { 
    return c[i];
}
 ////////////////                       
template< typename T> void f(T param){}
///////////
/* error: ‘x’ was not declared in this scope
auto f11() -> decltype(x) {
    int x =1;
    return x;
}
*/

decltype(auto) f21()  {
    int x =1;
    return x;
}

decltype(auto) f22()  {// !!! return int& => BAD memory problem!
    int x =1;
    return (x);
}

int main(int argc, char* argv[])
{
   
    /////
 // template with parameter
   //f({ 11, 23, 9 }); // error! can't deduce type for T
    //////////////
    //// {} init
    auto x1(5);// x1 type int
    auto x2{5};//x2 type std::initilazer_list<int>
    std::vector<int> v1(2,5);// vector {5.5}
    std::vector<int> v2{2,5};// vector {2,5}
    ////////////
    decltype(auto) y21 = f21(); 
    decltype(auto) y22 = f22();// !!! return int& => BAD memory problem!
    int x(1);
    decltype(auto) x22 = (x) ;// type of x - int, (x) - int&, x22 - int&

 return 0;
}
 
