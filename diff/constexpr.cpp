#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>
#include <iostream>
#include <exception>
#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<list>
#include<vector>
#include<queue>
#include<algorithm>
#include <type_traits>
#include <string>

constexpr int add(int a, int b)
{
	return a + b;
}

struct Av{
	constexpr Av(double l, double r) : m_l(l), m_r(r){}
	constexpr int get() { return (m_l + m_r) / 2.0; }
	double m_l;
	double m_r;
};

struct C2{
	static constexpr int i{ 25 + 7 };
};

template <int i> struct CL{};

enum C { X = add(5, 6) };
int len = std::string("xxxxxxxx").length();

//enum C2 {X2 = add(5 ,std::string("xxxxxxxx").length()) }; //error


CL<add(5, 6)> c1;
//CL<add(5 ,add(5 ,std::string("xxxxxxxx").length())> c1; // error

constexpr Av av1(6, 8);
Av av2(6, std::string("xxxxxxxx").length());
//constexpr Av av3(6, std::string("xxxxxxxx").length()); // error


CL<av1.get()> cl3;
int i = av2.get();
//CL<av2.get()> cl4; // error

CL<C2::i> sl5;

constexpr static int foo(int x) { return x + 1; }
class C4 {
	//constexpr static int foo(int x) { return x + 1; } /// error
	/*
	* This isn't possible, because unfortunately the
	* Standard precludes initializing a static constexpr
	* data member in any context where the class is complete.
	* The special rule for brace-or-equal-initializers in
	* 9.2p2 only applies to non-static data members,
	* but this one is static.

	The most likely reason for this is that constexpr variables
	have to be available as compile-time constant expressions
	from inside the bodies of member functions, so the variable
	initializers are completely defined before the function
	bodies -- which means the function is still incomplete
	(undefined) in the context of the initializer, and then this
	rule kicks in, making the expression not be a constant
	expression
	*
	*/
	constexpr static int bar{ foo(sizeof(int)) };
	//constexpr static int bar { sizeof(int) };
};

int main(int, char*[])
{



	return 0;
}
