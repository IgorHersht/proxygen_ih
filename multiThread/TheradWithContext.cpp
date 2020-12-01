#include <string>
#include <set>
#include <memory>
#include <iostream>
#include <thread>
#include <type_traits>

template< typename F, typename ContextT, typename ... Args> struct TheradWithContext {
    TheradWithContext(F&& f, ContextT context, Args&& ... args)  :
        m_thread(std::move(f), std::move(context), std::forward<Args>(args) ...)
    {
    }
    ~TheradWithContext(){
        try { 
            m_thread.join();
        }catch (...) {}
    }
private:
    std::thread m_thread;
};

void foo(std::unique_ptr< std::string> c, int i) {

    std::string out = *c + std::to_string(i);
    int j = 1;

}


int main()
{
    {
        TheradWithContext t(foo, std::make_unique<std::string>("aaa"), 25);
    }
   


    return 0;
}