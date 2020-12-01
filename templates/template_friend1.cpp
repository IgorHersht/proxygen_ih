
#include <string>
#include <vector>
#include <iostream>


template<class T>
struct B {
	friend void f(B b1){}
	friend void f(){}

};


int main() {
	B<int> b1;
	//B<double> b2; // error double void f() definition; 
	B<int> b2;
	f(b1);
	//f(); //error
}

