

#include <iostream>
#include <vector>

#include <boost/variant.hpp>
#include "boost/variant/variant.hpp"
#include "boost/variant/apply_visitor.hpp"
#include "boost/variant/static_visitor.hpp"

enum IDE {id0, id1,id2 };
template <IDE ID> struct Impl;
template <> struct Impl<id0>{};
template <> struct Impl<id1>{};





struct T1{
	void run(int &v){ std::cout <<"T1::run" << std::endl; v = 1; }
	void run2(){ std::cout <<"T1::run2" << std::endl; }

};

struct T2{
	void run(int &v){ std::cout <<"T2::run" << std::endl; v =2; }
	void run2(){ std::cout <<"T2::run2" << std::endl; }
};

typedef boost::variant<T1, T2> VT;


struct runVisitor: public boost::static_visitor<>{
	runVisitor(int &v):m_v(v){}
    template <typename T> void operator()( T & operand) const{
    	operand.run(m_v);
    }

    int &m_v;
};

struct run2Visitor: public boost::static_visitor<>{
    template <typename T> void operator()( T & operand ) const{
    	operand.run2();
    }
};


int main(int argc, char** argv){
	VT v1 = T1();

	VT v2 = T2();
	//VT v3 = T3();
	int j =0;
	boost::apply_visitor( runVisitor(j), v1 );
	boost::apply_visitor( run2Visitor(), v1 );
	boost::apply_visitor( runVisitor(j), v2 );
	boost::apply_visitor( run2Visitor(), v2 );





 return 0;
}

