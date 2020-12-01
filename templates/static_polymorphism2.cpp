#include <deque>
#include <list>
#include <set>
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <queue>
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
#include <string>

template <class Derived>
struct base {
  void foo() {
    static_cast<Derived *>(this)->foo();
  };
};

struct my_type : base<my_type> {
	void foo(){}// required to compile.
};

struct your_type : base<your_type> {
	void foo(){}; // required to compile.
};
//The second one is by avoiding the use of the reference-to-base or pointer-to-base 
//idiom and do the wiring at compile-time. Using the above definition, 
//you can have template functions that look like these:

template <class T> // T is deduced at compile-time
void bar(base<T> & obj) {
  obj.foo(); // will do static dispatch
}

struct not_derived_from_base { }; // notice, not derived from base

int main() 
{
	// ...
my_type my_instance;
your_type your_instance;
not_derived_from_base invalid_instance;
bar(my_instance); // will call my_instance.foo()
bar(your_instance); // will call your_instance.foo()
//bar(invalid_instance); // compile error, cannot deduce correct overload
	return 0;
}


//So combining the structure/interface definition and the compile-time type deduction 
//in your functions allows you to do static dispatch instead of dynamic dispatch. T
//his is the essence of static polymorphism.
	