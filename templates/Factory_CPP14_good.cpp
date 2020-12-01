



#include <iostream>
#include <type_traits>
#include<memory>
#include<string>
#include<vector>

template <typename T> struct Deleter : public std::default_delete<T>{};

template <typename BaseT, typename DeleterT = Deleter<BaseT>, typename PtrT =std::unique_ptr<BaseT,DeleterT>  > 
   struct Factory{
   using base_type = BaseT;
   using  pointer_type = PtrT;
   using  deleter_type = DeleterT;
   static_assert(std::has_virtual_destructor<base_type>::value, "base should have virtual_destructor ");
  
    template <typename ImplT, typename ... ArgsT > 
     static pointer_type make (deleter_type &dl, ArgsT&& ... args){
         static_assert(std::is_base_of<BaseT, ImplT>::value, "wrong impl type");
         static_assert(std::is_constructible<ImplT,  ArgsT...>::value, "wrong impl constructor");
         return pointer_type ( new ImplT(std::forward<ArgsT>(args)... ), dl ); 
     }
};


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



template <> struct Deleter<Base> : public std::default_delete<Base>{
    void operator ()(Base * base){
           if(base){
             base->close();
             std::default_delete<Base>::operator ()(base);
            } 
       }
};

Deleter<Base> del1;

auto del2 = [](Base * base){
     base->close();
     delete base;
};
        
//////////////////
using MyBaseFactory = Factory<Base > ;
using MyBasePtr = MyBaseFactory::pointer_type;

using MyBaseFactory2 = Factory<Base, decltype(del2) > ;
using MyBasePtr2 = MyBaseFactory2::pointer_type;
////////////////////

void foo(MyBasePtr b){
   b->run(); 
}

void foo2(MyBasePtr2 b){
   b->run(); 
}
template <typename PtrT>
void foo3(PtrT b){
   b->run(); 
}






int main(int argc, char* argv[]){
    
    {
        MyBasePtr ptr1;
        auto  imp1( Factory<Base>::make<Impl1>(del1, 5));
        imp1->run();
        int sz = sizeof(imp1); //8
        int sz2 = sizeof(ptr1); //8
        auto  impl2( Factory<Base>::make<Impl2>(del1, std::string("xxxx")));
        impl2->run();
        auto ptr2(Factory<Base,decltype(del2)>::make<Impl1>(del2, 10));
        ptr2->run();
        std::cout << " done test1)" << std::endl;
    }
    foo(MyBaseFactory::make<Impl1>(del1, 5));
    foo3(MyBaseFactory::make<Impl1>(del1, 5));
    std::vector<MyBasePtr> v1;
     v1.push_back( Factory<Base>::make<Impl1>(del1, 5));
     
     
    /* objects of the same lambda  - different types => 
     * different  deleter types => different  pointer types
     * not possible to have such pointers  as function parameters
     */
     std::vector<MyBasePtr2> v2;
    //foo2(MyBaseFactory::make<Impl1>(del2, 5));
   //  foo3(MyBaseFactory::make<Impl1>(del2, 5));
     //v2.push_back( Factory<Base>::make<Impl1>(del2, 5));
    
    
 return 0;
}



