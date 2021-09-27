
#ifndef _MAKE_UNIQUE_H
#define  _MAKE_UNIQUE_H

#include <memory>

template<typename T, typename Dp = std::default_delete<T>, typename... Args>
std::unique_ptr<T, Dp> make_unique(Args&&... args) {
  return std::unique_ptr<T, Dp>(new T(std::forward<Args>(args)...));
}

#endif
/*
int main(){
  std::unique_ptr<int> pi =  make_unique<int>(5);
  return 0;
}
*/