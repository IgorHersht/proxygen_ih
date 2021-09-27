
#include <thread>
#include <iostream>
#include <memory>
#include<string>
#include <queue>
#include <boost/variant.hpp>
#include "boost/variant/variant.hpp"
#include "boost/variant/apply_visitor.hpp"
#include "boost/variant/static_visitor.hpp"

struct Investment {
  virtual void log() = 0;
  virtual ~Investment() {}
};
////
enum class InvestmentID{StockID, BondId};

struct InvestmentDeleter{
  void operator()(Investment* in){
    if(in){
      in->log();
      delete in;
    }
  }
};
using InvestmentPtr = std::unique_ptr<Investment,InvestmentDeleter>;
////////
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
//////
template<typename ... Args> inline InvestmentPtr makeInvestment(InvestmentID id,  Args&& ...  args){
  InvestmentDeleter del;
  InvestmentPtr ip(nullptr, del) ;
  switch(id){
    case InvestmentID::StockID :{
      ip.reset( new Stock(std::forward<Args>(args) ...));
      break;
    }case InvestmentID::BondId:{
      ip.reset( new Bond(std::forward<Args>(args) ... ));
      break;
    }
  };
  return ip;
}
////
int main()
{
  std::queue<InvestmentPtr> ins;
  {
    ins.emplace(makeInvestment(InvestmentID::StockID));
    ins.emplace(makeInvestment(InvestmentID::BondId));
    ins.pop();
    ins.pop();
  }

  int j =1;
  return 0;
}

