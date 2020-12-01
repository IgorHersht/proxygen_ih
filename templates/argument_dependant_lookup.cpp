
#include <thread>
#include <condition_variable>
#include <mutex>
#include <future>
#include <memory>
#include <atomic>

#include <stack>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <string>

#include <algorithm>
#include <iterator>
#include <functional>
#include <numeric>

#include <boost/noncopyable.hpp>
#include <boost/thread/shared_mutex.hpp>


struct X {
static void f(int i){ std::cout  << "X::f; " << i<< std::endl; }
static int i;
};

int X::i(10);

namespace B{
enum E {e = 3};
void foo(E i){ std::cout  << "X::f; " << (int)i <<std::endl; }

};





int main(){

  //f(X::i); // error
  foo(B::e);


	return 0;
}
