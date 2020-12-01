#include <iostream>
#include <tuple>
#include <utility>


template <typename F, class... Args> struct C{

    C(F&& f, Args&& ... args):
            m_f(std::forward<F>(f)),
            m_args(std::forward<Args>(args) ... ){
    }
    void operator()() {

        std::apply( std::forward<F>(m_f), std::forward<decltype(m_args)>(m_args ));
    }
private:
    F m_f;
    std::tuple<Args ...> m_args;
};

void foo(int i, double d){

}


int main()
{
    C c(&foo,5, 4.8);

   c();
   
    return 0;
}

