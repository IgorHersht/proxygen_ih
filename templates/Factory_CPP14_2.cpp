

#include <iostream>
#include <type_traits>
#include<memory>
#include<string>



struct  Base{
    virtual ~Base() {};
    virtual void run() = 0;
    virtual void close() = 0;    
};

struct Impl1 : public Base{
   explicit Impl1(int data):Base(),m_data(data){std::cout << " Impl1()" << std::endl;}
    virtual ~Impl1()  {std::cout << " ~Impl1()" << std::endl;}
    virtual void run() override {std::cout << " Impl1::run() with " << m_data << std::endl;}
    virtual void close() override{std::cout << " Impl1::close()" << std::endl;}  
private:
    int m_data; 
};

struct Impl2 : public Base{
   explicit Impl2(std::string data):Base(),m_data(data){std::cout << " Impl2()" << std::endl;}
    virtual ~Impl2()  {std::cout << " ~Impl2()" << std::endl;}
    virtual void run() override {std::cout << " Impl2::run()with " << m_data << std::endl;}
    virtual void close() override{std::cout << " Impl2::close()" << std::endl;}  
private:
  std::string m_data; 
};

template <typename BaseT> struct Factory{
   static_assert(std::has_virtual_destructor<BaseT>::value, "base should have virtual_destructor ");
   struct deleter{
       auto operator ()(BaseT * base){
           if(base){
             base->close();
             delete base;
            } 
       }
   };

    using BasePtr = std::unique_ptr<BaseT, deleter >; 
    template <typename ImplT, typename ... ArgsT > 
     static BasePtr  make (ArgsT&& ... args){
         deleter dl;
         static_assert(std::is_base_of<BaseT, ImplT>::value, "wrong impl type");
         static_assert(std::is_constructible<ImplT,  ArgsT...>::value, "wrong impl constructor");
         return BasePtr ( new ImplT(std::forward<ArgsT>(args)... ), dl ); 
     }
};


void foo(Factory<Base>::BasePtr b){
   b->run(); 
}



int main(int argc, char* argv[]){
    
    {
        auto  imp1( Factory<Base>::make<Impl1>(5));
        imp1->run();
        auto  impl2( Factory<Base>::make<Impl2>(std::string("xxxx")));
        impl2->run();
        std::cout << " done test1)" << std::endl;
    }
    foo(Factory<Base>::make<Impl1>(5));
    
 return 0;
}

