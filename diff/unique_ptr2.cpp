 

#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <functional>
#include <numeric>
#include <vector>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>


#ifndef _MAKE_UNIQUE_H
#define  _MAKE_UNIQUE_H

namespace tools
{

 template <class T, typename ...Args>  
 inline std::unique_ptr<T> make_unique(Args ...args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
}
#endif

template <typename KeyT, typename ValueT> class MM{
    using Ptr  = std::unique_ptr<ValueT> ;
    using MapT = std::map<KeyT, Ptr>;
    using VT = typename MapT::value_type;  
   
   public:
       void insert(KeyT k, Ptr p  ){
            m_m.insert(VT(k, std::move(p)));          
       }
       
       bool exist(KeyT k){
           auto p = m_m.find(k);
           return( p != m_m.end());
       }
   private:
      MapT                m_m;
};

MM<int,std::string> m;



int main(int, char*[])
{
    m.insert(5,tools::make_unique<std::string> () );
    bool b1 =  m.exist(5);
    bool b2 =  m.exist(6);
    return 0;
}
