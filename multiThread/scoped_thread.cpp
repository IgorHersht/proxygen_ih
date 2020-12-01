#include <thread>
#include <exception>
class scoped_thread
{
std::thread t;
public:
explicit scoped_thread(std::thread t_):

t(std::move(t_))
{
if(!t.joinable())
throw std::logic_error("No thread");
}
~scoped_thread()
{
t.join();

}
scoped_thread(scoped_thread const&)=delete;
scoped_thread& operator=(scoped_thread const&)=delete;
};
void func(){
        int i =1;
}

void f()
{

scoped_thread t{std::thread(func)};

}

