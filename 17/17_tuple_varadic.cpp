#include <iostream>
#include <tuple>
#include <utility>
#include <memory>

template <class... Args> struct C{
  C( Args&& ... args):
      m_args(std::forward<Args>(args) ... ){
  }
  template <typename F> void invoce(F&& f  ) {
    std::apply(std::forward<F>(f), std::forward<decltype(m_args)>(m_args) );
  }
  void internal(int i, double d){
    std::cout << i <<" " << d << std::endl;
  }
private:
  std::tuple<Args ...> m_args;
};

/// test
void global(int i, double d){
  std::cout << i <<" " << d << std::endl;
}
struct A{
  void member(int i, double d){
  std::cout << i <<" " << d << std::endl;
  }
};


int main()
{
  C c( 5, 4.8);
  c.invoce( global );
  c.invoce( [a = std::make_unique<A>()](int i, double d) { a->member(i,d) ; });
  c.invoce( [&c](int i, double d) { c.internal(i,d) ; });
  return 0;
}




