#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

template <typename T> struct X{
	friend void _foo__(){}	
};





int main() 
{
	X<int> x1;
	X<int> x2; //OK
	//X<float> x3; //error
	return 0;
}

