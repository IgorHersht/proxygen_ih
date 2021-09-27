
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>
#include <iostream>
#include <exception>
#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<list>
#include<vector>
#include<queue>
#include<algorithm>
#include<iterator>
#include<string>

struct Sq{
    constexpr Sq(int l1 = 0, int l2 = 0 ) noexcept
            :m_sq(l1 * l2){}
    constexpr int get() const noexcept{
        return m_sq;
    }
private:
    int m_sq;
};

int f(int i){return 5 + i ;}



int main(int argc, char* argv[]){
    enum e { v= Sq(7, 4).get() };
    int i = v;
    int j = Sq(f(5), 4).get();
    //enum e2 { v= Sq(f(5), 4).get() }; //error
    
    
 return 0;
}
 
