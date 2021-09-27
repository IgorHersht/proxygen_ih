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


struct Stock{

	void log() {
		std::cout << "Stock::log"<<std::endl;
	}


};

struct  Bond{

	void log(){
			std::cout << "Bond::log"<<std::endl;
		}
};

typedef boost::variant<std::unique_ptr<Stock>, std::unique_ptr<Bond> >InvestmentPtr;

std::queue<InvestmentPtr> qq;

struct Visitor: public boost::static_visitor<>{
    template <typename T> void operator()( T & operand ) const{
    	operand->log();
    }
};


int main(int argc, char** argv){
	qq.push(std::make_unique<Stock>());
	qq.push(std::make_unique<Bond>());

	InvestmentPtr out = std::move(qq.front());
	qq.pop();
	boost::apply_visitor(Visitor(),out);
	out = std::move(qq.front());
	qq.pop();
	boost::apply_visitor(Visitor(),out);

 return 0;
}


