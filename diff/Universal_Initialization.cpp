

#include <string>
#include <array>
#include <initializer_list>
#include <queue>
#include <vector>
/*
#include <iostream>
#include <thread>
#include <memory>
#include <mutex>
#include <deque>
#include <future>
#include <memory>
#include <functional>
#include <iostream>
#include <iostream>

#include <thread>
#include <future>
#include <atomic>
#include <string>
#include <exception>
#include <stdexcept>
#include <functional>
#include <utility>

#include <boost/thread/thread.hpp>
#include <boost/thread/once.hpp>
#include <iostream>

#include <atomic>
#include <memory>
*/
struct P { int m_x; int m_y; };
struct P2 { int m_x = 1; int m_y = 2; };

template<typename T> struct  Widget {
 Widget(std::initializer_list<T> il):m_v(il){};
private:
  std::vector<T> m_v;
};


P makePoint(int x, int y) { return { x, y }; }

int getFirst(const std::vector<int>& v){
  return v.at(0);
}
 
int main() {
   
const P p {1, 2};
const std::vector<int> cv { 11, 20, 25 };
int i = cv[1];

//const std::array<int, 2> ca { 11, 20, 25 };// compile error too many initializers
const std::array<int, 2> ca2 { 11 };// {11, 0}

Widget<int> w1 { -55, 25, 16 };

const P p2 = makePoint(2, 5);
int j = getFirst( {2 ,5, 7});


    return 0;
}

