

#include <memory>
#include <map>
#include <string>
#include <mutex>
#include <boost/thread/shared_mutex.hpp>

template<typename T, typename Dp = std::default_delete<T>, typename... Args>
inline std::unique_ptr<T, Dp> make_unique(Args&&... args) {
  return std::unique_ptr<T, Dp>(new T(std::forward<Args>(args)...));
}


template <
            typename KeyT,
            typename ValueT,
            typename ContainerT = std::map<KeyT, std::unique_ptr<ValueT> >
          >
struct Cache{
const std::shared_ptr<ValueT> find(KeyT key ) const
{
  boost::shared_lock<boost::shared_mutex> lk( m_entry_mutex);
  auto pos = m_container.find(key);
  return ((pos != m_container.end()) ? std::make_shared<ValueT>(*(pos->second)):std::shared_ptr<ValueT>());
}
ValueT getCopy(KeyT key ) const
{
  boost::shared_lock<boost::shared_mutex> lk( m_entry_mutex);
  auto pos = m_container.find(key);
  return ( (pos !=  m_container.end()) ? *(pos->second):ValueT());
}

template <class ...Args>
void update_or_add(KeyT key, Args ...args){
  auto value = make_unique<ValueT>(args... );
  std::unique_lock<boost::shared_mutex>(m_entry_mutex);
  m_container[key]= std::move(value);
}

private:
  ContainerT                    m_container;
  mutable boost::shared_mutex   m_entry_mutex;

};

int main(){
  Cache<std::string, int> c1;
  c1.update_or_add("x", 1); c1.update_or_add("x", 2); c1.update_or_add("x", 1); c1.update_or_add("y", 3);
  auto r1 = c1.getCopy("x");
  auto r2 = c1.getCopy("y");

  return 0;
}