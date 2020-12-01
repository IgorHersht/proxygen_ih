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


struct Data {
	alignas(std::hardware_destructive_interference_size) int valueForOneThread;
	alignas(std::hardware_destructive_interference_size) int valueForAnotherThread;
};

struct Data2 {
	int valueForOneThread;
	int valueForTheSameThread;
};
// double-check we have best performance due to shared cache-line:
static_assert(sizeof(Data2) <= std::hardware_constructive_interference_size);
// ensure objects are properly aligned:
alignas(sizeof(Data2)) Data2 myDataForAThread;




int main()
{
	int s = sizeof(Data);
	

	return 0;
}

