#include <string>
#include <set>
#include <exception>
#include <iostream>
#include <thread>

inline static thread_local int i = 1;
void foo() {
    
    i++;
    int j = 1;
    int p = 0;
}



int main()
{
    
    std::thread t1(foo);
    std::thread t2(foo);
    std::thread t3(foo);
    t1.join(); t2.join();
    t3.join(); 


    return 0;
}

 
