#include <iostream>
#include <tuple>
#include <utility>
struct A{

    void foo(int i, double d){

    }
};

template <typename F, class... Args> struct C{

    C( F&& f,  Args&& ... args):
            m_f(std::forward<F>(f)),
            m_args(std::forward<Args>(args) ... ){
    }
  template <typename T> void operator()(T&& t) {
        auto tpl= std::tuple_cat(std::make_tuple(&std::forward<T>(t)), m_args );
        std::apply(std::forward<F>(m_f), std::forward<decltype(tpl)>(tpl ) );
    }
private:
    F m_f;
    std::tuple<Args ...> m_args;
};


int main()
{
    A a;
    C c( &A::foo,5, 4.8);

    c(a);

    return 0;
}

