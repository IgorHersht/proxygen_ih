#include <mutex>
#include <stdexcept>
#include <climits>
/*
 * The idea is that
you divide your application into layers and identify all the mutexes that may be locked
in any given layer. When code tries to lock a mutex, it isn’t permitted to lock that mutex
if it already holds a lock from a lower layer.
 The implementation of hierarchical_mutex uses a thread-local variable to store
the current hierarchy value.
 */

class hierarchical_mutex
{
    std::mutex internal_mutex;
    unsigned long const hierarchy_value;
    unsigned long previous_hierarchy_value;
    static thread_local unsigned long this_thread_hierarchy_value;

    void check_for_hierarchy_violation()
    {
        if(this_thread_hierarchy_value <= hierarchy_value)
        {
            throw std::logic_error("mutex hierarchy violated");
        }
    }
    void update_hierarchy_value()
    {
        previous_hierarchy_value=this_thread_hierarchy_value;
        this_thread_hierarchy_value=hierarchy_value;
    }
public:
    explicit hierarchical_mutex(unsigned long value):
            hierarchy_value(value),
            previous_hierarchy_value(0)
    {}
    void lock()
    {
        check_for_hierarchy_violation();
        internal_mutex.lock();
        update_hierarchy_value();
    }
    void unlock()
    {
        this_thread_hierarchy_value=previous_hierarchy_value;
        internal_mutex.unlock();
    }
    bool try_lock()
    {
        check_for_hierarchy_violation();
        if(!internal_mutex.try_lock())
            return false;
        update_hierarchy_value();
        return true;
    }
};
thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);

//test

hierarchical_mutex high_mutex(1000);
hierarchical_mutex low_mutex(500);
void low_handle();
void high_handle() {
    std::lock_guard lock(high_mutex);
    std::this_thread::sleep_for(std::chrono::microseconds(2000));
    low_handle();
}
void thread_a() {
    high_handle();
}

void low_handle() {
    std::lock_guard lock(low_mutex);
    std::this_thread::sleep_for(std::chrono::microseconds(1500));
}
void thread_b() {
    hierarchical_mutex now_mutex(100);
    std::lock_guard now_lock(now_mutex);

    high_handle();
}

int main()
{
    std::thread a(thread_a);
    std::thread b(thread_b);
    a.join();
    b.join();
}

