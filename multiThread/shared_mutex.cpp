
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include  <memory>
#include  <cstdio>
#include <mutex>
#include <boost/thread/shared_mutex.hpp>
#include <thread>
#include <thread>

struct th_string{

  th_string(){}
  th_string(const std::string &str):m_str(str){}
  th_string(std::string &&str):m_str(std::move(str)){}
  th_string& operator = (th_string &&str) = delete;
  th_string& operator =(const th_string &str) = delete;

private:
  boost::shared_mutex m_mutex;
  std::string m_str;
};

template <typename T> struct th_map{

  T find (const std::string key) const {
     boost::shared_lock<boost::shared_mutex> lk(m_mutex);
     auto pos = m_map.find(key);
     return (pos != m_map.end()) ?  pos->second : T();
  }

  bool insert (const std::string key, T value)  {
     std::lock_guard<boost::shared_mutex> lk(m_mutex);
     return (m_map.insert( typename std::map<std::string,T>::value_type(key,value) )).second;
  }

private:
  mutable boost::shared_mutex       m_mutex;
  std::map<std::string, T >  m_map;
};




int main()
{

  std::string s1("xxxx");
  th_map<int> m;
  m.insert("xxx", 1);  m.insert("cx", 6);  m.insert("x", 8);
  auto v = m.find("cx");

 
  return 0;
}

