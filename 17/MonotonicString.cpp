

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cstdlib>  // for std::byte
#include <memory_resource>
#include "tracknew.h"

template <size_t Sz>struct MonotonicString {
    using Type = std::basic_string<char, std::char_traits<char>, std::pmr::polymorphic_allocator<char> >;
    MonotonicString(const char* pos, size_t count):_str(pos, count, &_pool){
    }
    char* data() {
        return _str.data();
    }
    size_t size() const {
        return _str.size();
    }
    char& operator[]( size_t pos ){
        return *(data() + pos);
    }
private:
    std::array<std::byte, Sz> _buf;
    std::pmr::monotonic_buffer_resource _pool{_buf.data(), _buf.size() };
    Type _str;
};



void test0(const std::string& in){
    TrackNew::reset();
    MonotonicString<32> str(in.data(), in.size());
    for(size_t pos = 0; pos < str.size(); ++pos ){
        str[pos]  = std::tolower(str[pos]);
    }
    TrackNew::status();
    std::string tt(str.data(), str.size());
    int i =1;
}



int main()
{
    test0("5Ec*b");
}

