

#include <string>
#include <deque>
#include <vector>
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
#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>
#include <functional>
#include <utility>

int f(int i){
  return i;
}

struct C{
  int f(int i){
  return i;
  }
};

std::function<int(int)> fob;
std::function<int(C&, int)> mfob;

int main()
{
  fob = f;
  mfob = &C::f;
  int i = fob(10);
  C c1;
  int j = mfob(c1, 15);

  return 0;
    
}




