

#include <string>
#include <array>
#include <initializer_list>
#include <queue>
#include <vector>
#include <list>
#include <deque>

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

class Widget {
public:
void doSomething1() {
  auto it = std::find_if(li.cbegin(), li.cend(),
                         [this](int i) { return i > minVal; }
  );
}

void doSomething2() {
  auto it = std::find_if(li.cbegin(), li.cend(),
                         [&](int i) { return i > minVal; }
  );
}

private:
std::list<int> li;
int minVal;
};



// fine


std::vector<int> v;

int main() {
  {//local
int minVal;
double maxVal;

auto it = std::find_if(v.cbegin(), v.cend(),
  [minVal, &maxVal](int i) { return i > minVal && i < maxVal; }
);

  auto it2 = std::find_if(v.cbegin(), v.cend(),
                         [&](int i) { return i > minVal && i < maxVal; }
  );

  auto it3 = std::find_if(v.cbegin(), v.cend(),
                         [=](int i) { return i > minVal && i < maxVal; }
  );
}


    return 0;
}

