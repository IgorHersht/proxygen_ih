 

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
#include <deque>
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

/*
 Move functions in stl containers make comtainers moved from empty
 * (not sprecifie by standard)
 * 
 */

struct myset{
   myset(const std::set<int> s)
    {
      m_s =s;  
    }
void print()
{
    for(auto v : m_s)
        std::cout<< v <<":";
         std::cout<<std::endl;
}
    
    myset(myset&& ms) noexcept: m_s(std::move(ms.m_s)){}
    myset& operator = (myset&& ms){
        if(this == & ms) return *this;
        m_s = std::move(ms.m_s);
    }
    
   std::set<int> m_s; 
};


int main(){
    std::set<int> s1, s2; 
    s1.insert(1); s1.insert(3);  s1.insert(5);  s1.insert(7); 
    s2.insert(10); s2.insert(14);  
    
    myset ms1(s1), ms2(s2);
    ms1.print(); ms2.print(); 
    ms1 = std::move(ms2);
    ms1.print(); ms2.print(); 
    myset ms3 = std::move(ms1);
   ms3.print(); ms1.print();

	return 0;
}

/*
 * 
 * 
 1:3:5:7:
10:14:
10:14:

10:14:


 */
