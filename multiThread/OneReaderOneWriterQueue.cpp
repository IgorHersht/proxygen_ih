#include <atomic>
#include <cstddef>
#include <new>
#include <array>
#ifdef __cpp_lib_hardware_interference_size
using std::hardware_constructive_interference_size;
    using std::hardware_destructive_interference_size;
#else
// 64 bytes on x86-64 ¦ L1_CACHE_BYTES ¦ L1_CACHE_SHIFT ¦ __cacheline_aligned ¦ ...
constexpr std::size_t hardware_constructive_interference_size
        = 2 * sizeof(std::max_align_t);
constexpr std::size_t hardware_destructive_interference_size
        = 2 * sizeof(std::max_align_t);
#endif

template<typename T, std::size_t Size>
struct OneReaderOneWriterQueue {
    bool push(const T & value)
    {
        std::size_t head = head_.load(std::memory_order_relaxed);
        std::size_t next_head = next(head);
        if (next_head == tail_.load(std::memory_order_acquire))
            return false;
        ring_[head] = value;
        head_.store(next_head, std::memory_order_release);
        return true;
    }
    bool pop(T & value)
    {
        std::size_t tail = tail_.load(std::memory_order_relaxed);
        if (tail == head_.load(std::memory_order_acquire))
            return false;
        value = ring_[tail];
        tail_.store(next(tail), std::memory_order_release);
        return true;
    }
private:
    std::size_t next(std::size_t current)
    {
        return (current + 1) % Size;
    }
    std::array<T, Size> ring_;
    alignas(hardware_destructive_interference_size) std::atomic<std::size_t> head_{};
    alignas(hardware_destructive_interference_size) std::atomic<std::size_t> tail_{};
};


