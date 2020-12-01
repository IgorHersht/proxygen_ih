 

#include <thread>
#include <stack>
#include <mutex>
#include <memory>
#include <atomic>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <functional>
#include <numeric>
#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>
#include <typeinfo>
#include <algorithm>
#include <iterator>
#include <memory>
#include <stack>
#include <vector>
#include <condition_variable>

#include <boost/noncopyable.hpp>
#include <boost/thread/shared_mutex.hpp>

void f(){
    std::cout << "f1()"<< std::endl;
}

void f1(int i){
    std::cout << "f1(int " << i <<")"<< std::endl;
}

struct X{
    void mf1(double d)const {
        std::cout << "mf1(double " << d <<")"<< std::endl;
    }
};

typedef std::function<void () >  callback;

struct Y{
    
    void mf2(char ch){
        std::cout << "mf2(char " << ch <<")"<< std::endl; 
    }
     void mf3(){
         
             std::cout << "mf3(" << i <<")"<< std::endl;
         
    }
    
    void push (callback c){
        m_q.push_back(c);
    }
    void print()
    {
        for(auto c: m_q){
            c();
        }
    }
    
    int i{9};
    
  std::vector<callback> m_q;  
};

int main(){
        
        Y y;
        int i =5; double d = 4.0; char ch = 'x';
        y.push(f); 
        y.push(   [=]{f1(i); }  );
        X x;
        y.push(   [=]{x.mf1(d); }  );
        y.push(   [&]{y.mf2(ch); }  );
         y.push(  [&]{y.mf3(); }  );
        y.print();
	return 0;
}

