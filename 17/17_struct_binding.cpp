#include <boost/thread.hpp>
#include <memory>
#include <iostream>

#include <shared_lib/library_shared.h>
#include <static_lib/library_static.h>


struct MyStract{
    int i = 1;
    std::string data;
};

MyStract makeMyStruct(int i, std::string d ){
    return {3, "xxxx"};
}


void foo(){}
int main()
{
    const auto& [ in, dt] = makeMyStruct(5, "xxx");
    std::cout << in <<":" << dt << std::endl;
    return 0;
}
