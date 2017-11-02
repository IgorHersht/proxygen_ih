//Listing 5.12 Relaxed operations can be ordered with fences
#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x,y;
std::atomic<int> z;

void write_x_then_y()
{
    x.store(true,std::memory_order_relaxed);//B
    std::atomic_thread_fence(std::memory_order_release);//c
    y.store(true,std::memory_order_relaxed);//d
}

void read_y_then_x()
{
    while(!y.load(std::memory_order_relaxed));//e
    std::atomic_thread_fence(std::memory_order_acquire);//f
    if(x.load(std::memory_order_relaxed))//g
        ++z;
}

int main()
{
    x=false;
    y=false;
    z=0;
    std::thread a(write_x_then_y);
    std::thread b(read_y_then_x);
    a.join();
    b.join();
    assert(z.load()!=0);//h
}
/*
The release fence c synchronizes-with the acquire fence f , because the load from y
at e reads the value stored at d . This means that the store to x at B happens-before
the load from x at g , so the value read must be true and the assert at h won’t fire.
This is in contrast to the original case without the fences where the store to and load
from x weren’t ordered, and so the assert could fire. Note that both fences are neces-
sary: you need a release in one thread and an acquire in another to get a synchronizes-
with relationship.
*/

