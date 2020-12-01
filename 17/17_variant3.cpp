#include <iostream>
#include <memory>
#include<string>
#include <tuple>
#include<queue>
#include <variant>
#include <type_traits>

#include <boost/thread/concurrent_queues/sync_queue.hpp>
enum class InvestmentId {Stock, Bond};
template <InvestmentId id> struct Investment;
template<InvestmentId id> using TheInvestmentPtr = std::unique_ptr<Investment<id>>;
using  AnInvestment = std::variant< TheInvestmentPtr<InvestmentId::Stock>, TheInvestmentPtr<InvestmentId::Bond> >;

template <InvestmentId id, typename ... Args> inline AnInvestment makeInvestment( Args&& ... args){
    AnInvestment investment = TheInvestmentPtr<id>(new Investment<id>( std::forward<Args>(args) ... ));
    return investment;
}
////
boost::sync_queue<AnInvestment> investments;
///

template <> struct Investment<InvestmentId::Stock>{

    Investment(int par1, std::string par2):
            _par1(par1), _par2(std::move(par2)){

    }
    void foo(int i){
        std::cout << _par2 <<" price =" << price(i) << std::endl;
    }
private:
    int price(int i) {
        return i* _par1;
    }
private:
    int             _par1;
    std::string     _par2;
};

template <> struct Investment<InvestmentId::Bond>{
    Investment(int par1, std::string par2, std::string par3 ):
            _par1(par1), _par2(std::move(par2)), _par3(std::move(par3))
    {
    }

    void foo(int i){
        std::cout << _par2 << _par3<<" price =" << price(i) << std::endl;
    }
private:
    int price(int i) {
        return i* _par1;
    }
private:
    int             _par1;
    std::string     _par2;
    std::string     _par3;

};
/////
struct Visitor{
    Visitor(int v):m_v(v){}
    template <typename T> void operator()( T & inv) const{
        inv->foo(m_v);
    }

    int m_v;
};
///

int main()
{
    AnInvestment si = makeInvestment<InvestmentId::Stock>(2, "Stock");
    AnInvestment bi = makeInvestment<InvestmentId::Bond>(10, "Bond", "Fixed");
    investments.push(std::move(si));
    investments.push(std::move(bi));

    AnInvestment i1 = investments.pull();
    AnInvestment i2 = investments.pull();

    std::visit(Visitor(3), i1);
    std::visit(Visitor(7), i2);

    std::visit([i =5](auto& in){in->foo(i); }, i1);
    std::visit([i =11](auto& in){in->foo(i); }, i2);

    return 0;
}


