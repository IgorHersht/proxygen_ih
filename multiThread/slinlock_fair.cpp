#include <atomic>
#include <array>
#ifdef __cpp_lib_hardware_interference_size
using std::hardware_constructive_interference_size;
using std::hardware_destructive_interference_size;
#else
constexpr std::size_t hardware_constructive_interference_size = 64;
constexpr std::size_t hardware_destructive_interference_size = 64;
#endif

class spinlock_mutex
{
    alignas(hardware_destructive_interference_size) std::atomic<int> in {0};
    alignas(hardware_destructive_interference_size)std::atomic<int> out {0};
public:
    void lock()
    {
        const auto my = in.fetch_add(1, std::memory_order_acquire);
        while(true){
            const auto now = out.load(std::memory_order_acquire);
            if(now == my){
                return;
            }
            out.wait(now,std::memory_order_relaxed);
        }
    }
    void unlock()
    {
        out.fetch_add(1, std::memory_order_acquire);
        out.notify_one();
    }
};

int main(int, char*[])
{


    return 0;
}
