#include <array>
#include <atomic>
#include <vector>
#include<iostream>
#include<string>
#include<memory>


int main() {
    using Ar = std::array<std::array<int, 5>, 5>;
    auto  arPtr = new Ar{{}}; // zero inilized
    size_t s = sizeof(Ar);
    (*arPtr)[2][3] = 5;
    delete arPtr;
    for(auto & ar: (*arPtr)){
        for(auto & e: (ar)){
            auto p0= arPtr;
            auto p1 = &e;
            auto df = ( std::byte*)&e - ( std::byte*)arPtr;
            std::cout << df << std::endl;
        }
    }

    std::unique_ptr<Ar> arPtr2( new Ar{{}} ) ;
    std::unique_ptr<Ar> arPtr3 = std::make_unique<Ar>();

    int i = 1;


}

