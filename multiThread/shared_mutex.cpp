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
#include <fstream>
#include <vector>

#include <boost/noncopyable.hpp>
#include <shared_mutex>

struct thread_guard{
    thread_guard(std::thread &t):m_t(t){
    }
    ~thread_guard(){
        if(m_t.joinable()) {
            m_t.join();
        }
    }
private:
    thread_guard& operator = (const thread_guard &) = delete;
    thread_guard(const thread_guard &) = delete;
    std::thread &m_t;

};

template <typename KeyT, typename ValueT> class smap{
    using map = std::map<KeyT,ValueT>;
public:
    using value_type =  typename map::value_type;

    bool insert(value_type &&v){
        std::lock_guard lk(m_mutex);
        return (m_map.insert(std::move(v))).second;
    }

    std::pair<ValueT, bool> find (const KeyT &k) const {
        std::shared_lock lk(m_mutex);
        auto pos = m_map.find(k);
        return (pos != m_map.end()) ? std::make_pair(pos->second, true) : std::make_pair(ValueT(), false) ;
    }

private:
    mutable std::shared_mutex m_mutex;
    map                 m_map;
};

using map =smap<int,int>;
std::mutex IO_mutex;

inline void find(const map &m ){

    for (int i = 0; i < 1000; ++i) {
        auto out = m.find (i);
        std::lock_guard<std::mutex> lk(IO_mutex);
        std::cout << i <<";"<<out.second <<";"<< out.first <<std::endl;
    }
}


inline void insert ( map &m ){

    for (int i = 0; i < 1000; ++i) {
        m.insert(std::move(map::value_type(i/2 , i)) );
    }

}


int main(){
    map m;
    std::thread t1(insert, std::ref(m) );   thread_guard g1 (t1);
    std::thread t2(find, std::ref(m) );     thread_guard g2(t2);
    std::thread t3(find, std::ref(m) );     thread_guard g3(t3);

    return 0;
}
