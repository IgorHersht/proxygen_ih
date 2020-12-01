#include <thread>
#include <stack>
#include <mutex>
#include <memory>
#include <atomic>
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
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include <typeinfo>
#include <algorithm>
#include <iterator>
#include <memory>
#include <fstream>
#include <vector>

template<class T> using Vec = std::vector<T>;
Vec<int> vi;
using Int = int;
Vec<Int> vi2;

using vvf = void (*)();

void foo(){
std::cout <<"XXXX"<<std::endl;
}

vvf v1 =foo;

int main()
{

v1();
 
	return 0;
}
