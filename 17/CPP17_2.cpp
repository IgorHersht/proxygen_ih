 #include <initializer_list>
 #include <iostream>
/*
Lambda capture this by value

Capturing this in a lambda's environment was previously reference-only. 
An example of where this is problematic is asynchronous code using callbacks that 
require an object to be available, potentially past its lifetime. *this (C++17) will 
now make a copy of the current object, while this (C++11) continues to capture by reference.
*/

struct MyObj {
  int value{ 123 };
  auto getValueCopy() {
    return [*this] { return value; };
  }
  auto getValueRef() {
    return [this] { return value; };
  }
};
int main(){
    
    MyObj mo;
auto valueCopy = mo.getValueCopy();
auto valueRef = mo.getValueRef();
mo.value = 321;
std::cout << valueCopy() << std::endl; // 123
 std::cout << valueRef() << std::endl;// 321


}
