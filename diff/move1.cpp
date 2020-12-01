

#include <string>
#include <array>
#include <initializer_list>
#include <queue>
#include <vector>
#include <list>
#include <deque>

#include <thread>
#include <memory>
#include <mutex>
#include <future>
#include <memory>
#include <functional>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <algorithm>


struct WidgetBase {
  WidgetBase(){}
   WidgetBase(const WidgetBase &){
     std::cout <<" WidgetBase(const WidgetBase &"<<std::endl;
   }

   WidgetBase& operator=(const WidgetBase &rhs){
      std::cout <<" WidgetBase& operator=(WidgetBase &rhs)"<<std::endl;
      return *this;
   }
    WidgetBase(WidgetBase &&){
     std::cout <<" WidgetBase(const WidgetBase &&"<<std::endl;
   }

   WidgetBase& operator=(WidgetBase &&rhs){
      std::cout <<" WidgetBase& operator=(WidgetBase &&rhs)"<<std::endl;
        return *this;
   }

};



struct Widget: public WidgetBase {

  Widget():s("sxxx"), ps(new std::string("yyyy"))
  {
  }

  Widget(const Widget&rhs) : WidgetBase(rhs), s(rhs.s), ps(new  std::string( *(rhs.ps) ) )  {
  }

  Widget(Widget&& rhs) : WidgetBase(std::move(rhs)), s(std::move(rhs.s)), ps(rhs.ps) {
          rhs.ps = nullptr;
  }

   Widget& operator=(const Widget& rhs) {
      if(this == &rhs) {
        return *this;
      }

    WidgetBase::operator=(rhs);
    s = rhs.s;
    delete ps;
    ps = new std::string(*(rhs.ps));
    return *this;
  }
 
  Widget& operator=(Widget&& rhs){

  WidgetBase::operator=(std::move(rhs));
  s = std::move(rhs.s);
  ps = rhs.ps;
  rhs.ps = nullptr;
  return *this;
  }

  std::string s;
  std::string *ps;

};

int main() {
  

    return 0;
}

