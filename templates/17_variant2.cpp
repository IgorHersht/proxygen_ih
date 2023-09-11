#include <thread>
#include <iostream>
#include <memory>
#include<string>
#include <tuple>
#include<queue>
#include <variant>
#include <type_traits>

#include <boost/thread/concurrent_queues/sync_queue.hpp>

template<typename T> using TheInvestmentPtr = std::unique_ptr<T>;

struct VisitorFoo{
    VisitorFoo(int i):m_i(i){}
    template <typename T> void operator() (T inv){
        inv->foo(m_i);
    }
private:
    int m_i;
};



///
struct Stock{
    Stock(int par1, std::string par2):
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

struct  Bond{
    Bond(int par1, std::string par2, std::string par3 ):
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

using  AnInvestment = std::variant< TheInvestmentPtr<Stock>, TheInvestmentPtr<Bond> >;
boost::sync_queue<AnInvestment> q;

int main()
{
    q.push(std::make_unique<Stock>(2, "Stock"));
    q.push(std::make_unique<Bond>(2, "Bond", "Fixed"));


    std::visit(VisitorFoo(3), q.pull());
    std::visit(VisitorFoo(7), q.pull());
    ////
   

    return 0;
}
