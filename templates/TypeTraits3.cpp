 #include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;
typedef void (c::*pfm)();
template <typename CT, typename FT > struct Has{
	enum {Value = false};
};

template <typename CT > struct Has< CT:: pfm>{
	enum {Value = true};
};


int main() 
{
	
	return 0;
}

