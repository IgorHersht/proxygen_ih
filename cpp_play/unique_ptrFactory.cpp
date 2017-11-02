#include <thread>
#include <iostream>
#include <memory>
#include<string>
#include <tuple>
#include <boost/variant.hpp>
#include "boost/variant/variant.hpp"
#include "boost/variant/apply_visitor.hpp"
#include "boost/variant/static_visitor.hpp"



struct Investment {
	virtual void log() = 0;
	virtual ~Investment() {
	}

};

struct Stock:public Investment {

	void log()override {
		std::cout << "Stock::log"<<std::endl;
	}
	~Stock(){}

};

struct  Bond: public Investment {

	void log()override {
			std::cout << "Bond::log"<<std::endl;
		}
	~Bond(){

	}
};

enum class InvestmentID{StockID, BondId};

template<typename ... Args>
auto makeInvestment(InvestmentID id, Args&& ...args ){

	auto Deleter = [](Investment *pi){
		pi->log();
		delete pi;
	};

	std::unique_ptr<Investment, decltype(Deleter)> ip(nullptr, Deleter) ;
	switch(id){
		case InvestmentID::StockID :{
			 ip.reset( new Stock(std::forward<Args>(args) ...));
		}
		case InvestmentID::BondId:{
			 ip.reset( new Bond(std::forward<Args>(args) ... ));
		}
	};

	return ip;
}
/*

*/



int main(int argc, char** argv){

auto b = makeInvestment(InvestmentID::BondId);


 return 0;
}


