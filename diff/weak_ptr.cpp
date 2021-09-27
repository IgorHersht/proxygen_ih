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





int main()
{
	boost::shared_ptr<int> spw1(new int(5));
	boost::weak_ptr<int> wpw(spw1);
	{
		{
				boost::shared_ptr<int> pw2(wpw.lock());// #1  
				spw1.reset();
				bool ex0 = !spw1; int c1= spw1.use_count() ; // 0 true
				bool ex = !pw2;  int c2= pw2.use_count(); // 1 false line # 1 makes it NOT expired
				bool ex1 = wpw.expired(); int c3= wpw.use_count(); // 1 false line # 1 makes it NOT expired

				int i = 0;
		}

	}
	bool ex2 = wpw.expired();
	return 0;
}
