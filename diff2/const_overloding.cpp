#include <boost/thread/thread.hpp>
#include <boost/thread/once.hpp>
#include <iostream>
#include <iostream>
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
#include <iostream>
#include <typeinfo>
#include <algorithm>
#include <iterator>
#include <memory>

struct C {
	
	void f(int &) {
		std::cout << "void f(int &) " << std::endl;
	}
	void f(const int &){
		std::cout << "void f(const int &)" << std::endl;
	}
	void f(int &) const{
		std::cout << "void f(int &) const " << std::endl;
	}
};

void g(const int &){
		std::cout << "g(const int &) " << std::endl;
	}

void g(int &){
	std::cout << "g(int &) " << std::endl;
}



int main()
{

	int i;
	const int ci = 1;

C c1; 
c1.f(i);
c1.f(ci);
const C c2; 
c2.f(i);


g(i);
g(ci);
  
  return 0;
}
