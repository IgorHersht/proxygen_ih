#include <iostream>
#include <tuple>
#include <utility>
struct A{

    void foo(int i, double d){

    }
};

template <class... Args> struct C{

    C( Args&& ... args):
            m_args(std::forward<Args>(args) ... ){
    }
  template <typename T> void operator()(T t) {
        auto tpl= std::tuple_cat(std::make_tuple(&t), m_args );
        std::apply(&T::foo, std::forward<decltype(tpl)>(tpl ) );
    }
private:
    std::tuple<Args ...> m_args;
};


int main()
{
    A a;
    C c(5, 4.8);

    c(a);

    return 0;
}

