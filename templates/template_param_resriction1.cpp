#include <boost/thread/thread.hpp>
#include <boost/thread/once.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
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
#include <string>
#include <vector>
#include <queue>
#include <iostream>
using namespace std;
#include <string.h>

  template <typename T> class A {
	  enum Validate{v= (Validate)T::valid};
 };

  struct Good {
	  enum {valid};
  };
  struct Bad {
  };
A<Good> g;
//A<Bad> b;// compile error

int main()
{	
	return 0;
}
