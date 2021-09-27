

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

/*
Reference Collapsing 
 
Reference collapsing involving a & is always T&.
 Reference collapsing involving only && is T&&.
 
*/
int i;
typedef int& LRI;
typedef int&& RRI;

LRI& r1 = i; // r1 has the type int&
const LRI& r2 = i; // r2 has the type int&
const LRI&& r3 = i; // r3 has the type int&
RRI& r4 = i; // r4 has the type int&
RRI&& r5 = 5; // r5 has the type int&&
decltype(r2)& r6 = i; // r6 has the type int&
decltype(r2)&& r7 = i; // r7 has the type int&


int main(){



  return 0;
}

