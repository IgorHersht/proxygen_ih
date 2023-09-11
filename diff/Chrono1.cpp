#include <chrono>
#include <iostream>
#include <thread>


int main()
{
    using namespace std::literals; // enables the usage of 24h, 1ms, 1s instead of
    // e.g. std::chrono::hours(24), accordingly
    const std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    const std::chrono::time_point<std::chrono::system_clock> end1 = start + std::chrono::milliseconds(3) ;
    std::cout<< "d1 =" <<std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start) << std::endl;
    bool b = end1 > start;

    const std::chrono::time_point<std::chrono::system_clock> end2 = std::chrono::time_point<std::chrono::system_clock>::max() ;
    std::cout<< "d2 =" <<std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start) << std::endl;
    bool b2 = end2 > start;


    std::this_thread::sleep_for(std::chrono::milliseconds(3 ));

}


