#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

template <typename T> struct X{
	X(T t):t1(t){}
	T t1;
};

/// function
template < template <typename T> class Y >
double f(){
	Y<int> yi(1.5);
	Y<float> yf(1.5);
	return  yf.t1 - yi.t1;
}

template < template <typename > class Y >
struct A { // cannot be instantieted
	Y<int> yi;
	Y<float> yf;
};

/// class
template < typename T, template <typename > class Y >
struct C{
	C(): tt(T()){}
	Y<T> tt;
};

/// stl
template < typename T, template <typename T2, typename = std::allocator<T2> > class Container >
struct ConWr{


Container<T> c1;
};




int main()
{
	float d = f < X >();

	C<int, X> c;
	ConWr<int, vector> con1;
	return 0;
}
