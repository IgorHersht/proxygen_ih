#include <type_traits>
#include <iostream>
#include <memory>
#include<string>
#include <tuple>
#include<typeinfo>


template<  typename Ft, typename... Args > inline decltype(auto) callWithFanctor(  Ft&& f, Args&&... args ){

	//std::cout << "f is_rvalue_reference="<< std::is_rvalue_reference<decltype(f)>::value <<std::endl;
	//std::cout << "f is_lvalue_reference="<< std::is_lvalue_reference<decltype(f)>::value <<std::endl;
	return std::forward<Ft>(f)(std::forward<Args>(args)...);
}

auto callWithFanctor2 =
	[](auto&& f, auto&& ... args  ){
	return std::forward<decltype(f)>(f)(std::forward<decltype(args)>(args)...);
};

//////////////////////// test

struct C{
	C(){}
	C(const C&){
		std::cout << "C(const C&)"<<std::endl;
	}

	void f(C&&){

	}
};

struct F{
	int operator()(int j) const{
		return j;
	}
	F(){

	}
	F(const F&){
		std::cout << "F(const F&)"<<std::endl;
	}
	F& operator =(const F&){
			std::cout << "F& operator =(const F&)"<<std::endl;
		}
	F& operator =( F&&){
			std::cout << "F& operator =( F&&)"<<std::endl;
		}
	F( F&&){
		std::cout << "F( F&&)"<<std::endl;

	}
	std::unique_ptr<int> m_d{new int(5)};
};
int foo(int i, const int &cr, int &r, std::unique_ptr<int> &&up, C c){

	std::cout << "foo"<<std::endl;
	i++;
	r++;
	return i;
}

int main(int argc, char** argv){
	int j = 5;
	std::unique_ptr<int> up(std::make_unique<int>(45));
	C c;
	c.f(std::move(c));
	//c.f(c); // error

	int k = callWithFanctor(foo, 17, 2, j, std::move(up), c );
	std::cout << "k="<< k <<std::endl;
	std::cout << "j="<< j<<std::endl;
	int k2 = callWithFanctor2(foo, 17, 2, j, std::move(up), c );

	int && k3 =  callWithFanctor(foo, 17, 2, j, std::move(up), c );
	bool b1= std::is_rvalue_reference<decltype(k2)>::value;
	bool b2= std::is_rvalue_reference<decltype(callWithFanctor(foo, 17, 2, j, std::move(up), c ))>::value;

	const F f;
	//int i = f(5);
	int i = callWithFanctor(f, 5);//call with lvalue_reference
	int i2 = callWithFanctor(std::move(f), 5);//call with rvalue_reference
	int i3 = callWithFanctor2(f, 5);//call with lvalue_reference


	auto l= [j](int g){return j * g;};

	int s =  callWithFanctor(l, 5);
	int


 return 0;
}

