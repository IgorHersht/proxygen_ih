#include <boost/thread.hpp>
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


template<typename T> void sw(T &t1, T &t2){
	T temp = t1;
	t1 = t2;
	t2 = temp;
}

template<typename T> void sw(vector<T> &t1, vector<T> &t2){
	t1.swap(t2);
}


int main() 
{
	vector<int> v1(1,10);
	vector<int> v2(2,10);

	sw(v1, v2);
	return 0;
}                

