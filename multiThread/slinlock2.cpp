#include <atomic> 
#include <array> 

class spinlock_mutex
{
    std::atomic_flag flag {ATOMIC_FLAG_INIT};
public:
    void lock()
    {
        while(flag.test_and_set(std::memory_order_acquire)){
            flag.wait(true,std::memory_order_relaxed); 
        }
    }
    void unlock()
    {
        flag.clear(std::memory_order_release);
        flag.notify_one();

    }
};

int main(int, char*[])
{
   

    return 0;
}
