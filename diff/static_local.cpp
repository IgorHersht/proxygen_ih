#include <future>
#include <functional>
#include <chrono>
#include <optional>
#include <future>
#include <functional>
#include <chrono>
#include <optional>

struct C{
  int foo(){

    static int i = 0;
    i++;
    return i;
  }
};

//test
#include <iostream>



int main()
{

C c1, c2;
int i = c1.foo();
i = c2.foo();// i =2

  return 0;
}
}

