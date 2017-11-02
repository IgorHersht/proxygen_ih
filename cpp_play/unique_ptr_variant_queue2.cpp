#include <thread>
#include <iostream>
#include <memory>
#include<string>
#include <tuple>
#include<queue>
#include <boost/variant.hpp>
#include "boost/variant/variant.hpp"
#include "boost/variant/apply_visitor.hpp"
#include "boost/variant/static_visitor.hpp"

struct C{
	C(){}
	C(const C&){
		std::cout << "C(const C&)"<<std::endl;
	}

	void f(C&&){

	}

};


struct Stock{

	void log() {
		std::cout << "Stock::log"<<std::endl;
	}
	int foo(int i, const int &cr, int &r, std::unique_ptr<int> &&up, C c){

		std::cout << "Stock::foo"<<std::endl;
		i++;
		r++;
		return i;
	}



};

struct  Bond{

	void log(){
			std::cout << "Bond::log"<<std::endl;
	 }

	int foo(int i, const int &cr, int &r, std::unique_ptr<int> &&up, C c){

		std::cout << "Bond::foo"<<std::endl;
		i++;
		r++;
		return i;
	}


};

typedef boost::variant<std::unique_ptr<Stock>, std::unique_ptr<Bond> >InvestmentPtr;

std::queue<InvestmentPtr> qq;

 struct logVisitor: public boost::static_visitor<>{
    template < typename T >void operator()( T & operand ) {
    	return operand->log();
    }
};

 struct fooVisitor: public boost::static_visitor<int>{

	 fooVisitor(int i, const int &cr, int &r, std::unique_ptr<int> &&up, const C &c):
		m_i(i), m_cr(cr),m_r(r),m_up(std::move(up)), m_c(c){

	 }
     template < typename T > int operator()( T & operand )const{
     	return operand->foo(m_i,m_cr, m_r, std::move(m_up), m_c);
     }
 private:
     int m_i;
	 const int &m_cr;
	 int &m_r;
	 std::unique_ptr<int> &&m_up;
	 C m_c;

 };



int main(int argc, char** argv){
	qq.push(std::make_unique<Stock>());
	qq.push(std::make_unique<Bond>());

	InvestmentPtr out = std::move(qq.front());
	qq.pop();

	logVisitor lv;
	boost::apply_visitor(lv,out);

	int j = 5;
	C c;
	fooVisitor fv(17, 2, j, std::make_unique<int>(45), c);


	boost::apply_visitor(fv, out);


 return 0;
}



