
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
using namespace std;

struct S{
   
    void cp_if(vector<int> &from){
     // auto div = m_div;
      copy_if(begin(from), end(from), begin(m_data),
       [=](int val){
           bool out = (val % m_div == 0);
           if( val > m_div){
               return true;
           }
           return out;   
        }
       );
    }
   int m_div = 2 ; 
   vector<int> m_data = vector<int>(10,0);// init 10 elements with 0
};


int main(int argc, char* argv[])
{
  
    vector<int> v1{2,5, 4,1, 28};
    vector<int> v2(10,0);
    int d = 2;
    auto d_2 = ([=](int val){return (val % d == 0); } );
    bool allof1 = all_of(begin(v1), end(v1), [=](int val){return (val % d == 0); }  );
    bool allof2 = all_of(begin(v1), end(v1), d_2);
    //auto it = copy_if(begin(v1), end(v1), begin(v2), [d](int val){return (val % d == 0); }   );
    
    ///
      S s;
      s.cp_if(v1);
 return 0;
}
 
