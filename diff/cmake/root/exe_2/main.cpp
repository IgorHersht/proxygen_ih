

#include <boost/thread.hpp>
#include <memory>

#include <shared_lib/library_shared.h>

void foo(){}
int main()
{
    boost::thread th(foo);
    th.join();
    hello_shared();
}