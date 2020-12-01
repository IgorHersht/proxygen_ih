#include <initializer_list>

constexpr int ar[] = {1,2, 3};
enum { v1 = ar[1]};

constexpr auto il = {1,2, 3};
enum  e{ v2 = il.size()};


int main()
{
		int i = v1;
		return 0;
}


