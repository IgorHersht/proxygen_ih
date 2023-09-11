#include <atomic>
#include <new>
#include <array>

#ifdef __cpp_lib_hardware_interference_size
using std::hardware_constructive_interference_size;
    using std::hardware_destructive_interference_size;
#else
constexpr std::size_t hardware_constructive_interference_size = 64;
constexpr std::size_t hardware_destructive_interference_size = 64;
#endif

template<typename T, std::size_t Size>
struct OneReaderOneWriterQueue {
    bool push(const T& value)
    {
        std::size_t head = _head.load(std::memory_order_relaxed);
        std::size_t next_head = next(head);
        if (next_head == _tail.load(std::memory_order_acquire))
            return false;
        _ring[head] = value;
        _head.store(next_head, std::memory_order_release);
        return true;
    }
    bool pop(T& value)
    {
        std::size_t tail = _tail.load(std::memory_order_relaxed);
        if (tail == _head.load(std::memory_order_acquire))
            return false;
        value = _ring[tail];
        _tail.store(next(tail), std::memory_order_release);
        return true;
    }
private:
    std::size_t next(std::size_t current)
    {
        return (current + 1) % Size;
    }
    char _pad0[hardware_destructive_interference_size];
    std::array<T, Size> _ring;
    alignas(hardware_destructive_interference_size) std::atomic<std::size_t> _head{};
    alignas(hardware_destructive_interference_size) std::atomic<std::size_t> _tail{};
    char _pad1[hardware_destructive_interference_size - sizeof(std::atomic<size_t>)];
};
    // test
#include <vector>
#include <iostream>
#include <thread>

OneReaderOneWriterQueue<std::array<int, 2>, 256 > q;
    const  size_t capacity = 256;
    using  El = std::array<int, 2>;

    void push(){
        for(int i = 0; i < capacity; ++i){
            El el {i, i + int(capacity)};
            q.push(el);
        }
    }

    void pop(){
        std::vector<El> v;
        for(int i = 0; i < capacity; ++i){
            El el {};
            if(q.pop(el)){
                v.emplace_back(el);

            }


        }
        for(auto el: v){
            std::cout <<el[0] << ";" << el[1]<< std::endl;
        }
    }



int main() {
    std::thread t1(push);
    std::thread t2(pop);
    t1.join();
    t2.join();



}
