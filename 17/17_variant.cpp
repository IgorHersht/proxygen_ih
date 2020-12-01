#include <thread>
#include <iostream>
#include <memory>
#include<string>
#include <tuple>
#include<queue>
#include <variant>
#include <type_traits>

#include <boost/thread/concurrent_queues/sync_queue.hpp>

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

template<typename T> using TheInvestmentPtr = std::unique_ptr<T>;
using  AnInvestment = std::variant< TheInvestmentPtr<Stock>, TheInvestmentPtr<Bond> >;

template <typename T, typename ... Args> inline AnInvestment makeInvestment( Args&& ... args){

    if constexpr(
                std::is_same_v<Stock,T>
            ||  std::is_same_v<Bond,T>
            ) {
        AnInvestment investment{ std::make_unique<T> (T( std::forward<Args>(args) ... ))};
        return investment;
    }else{
        constexpr bool UnknownInvestment =  std::is_same_v<T, void>;
        static_assert(UnknownInvestment, "Unknown investment type");
    }
}


struct Visitor{
    Visitor(int v):m_v(v){}
    template <typename T> void operator()( T & inv) const{
        inv->foo(m_v);
    }

    int m_v;
};



boost::sync_queue<AnInvestment> q;
int main()
{
    AnInvestment investment;// a default-constructed variant holds a value of its first default-constructed alternative,
    constexpr size_t size = std::variant_size_v<AnInvestment>;//2

    AnInvestment si = makeInvestment<Stock>(2, "Stock");
    AnInvestment bi = makeInvestment<Bond>(10, "Bond", "Fixed");
    q.push(std::move(si));
    q.push(std::move(bi));

    AnInvestment i1 = q.pull();
    AnInvestment i2 = q.pull();

    std::visit(Visitor(3), i1);
    std::visit(Visitor(7), i2);

    std::visit([i =5](auto& in){in->foo(i); }, i1);
    std::visit([i =11](auto& in){in->foo(i); }, i2);


    return 0;
}

