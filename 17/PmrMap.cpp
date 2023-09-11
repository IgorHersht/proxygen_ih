#include <iostream>
#include <iostream>
#include <string>
#include <array>
#include <unordered_map>
#include <cstdlib>  // for std::byte
#include <memory_resource>
#include "tracknew.h"


template <typename K, typename V, size_t SZ> struct PmrMap{

    std::array<char, SZ> _buf;
    std::pmr::monotonic_buffer_resource _pool{_buf.data(), SZ };
    auto emplace(K&& k, V&& v) {
        return _map.emplace(k,v);
    }
public:
    using Map = std::pmr::unordered_map<K, V>;
    Map _map;
    PmrMap(): _map(&_pool){

    }

};

using IMap = std::unordered_map<int, int>;
using EMap = PmrMap<int, IMap, 10000>;

int main() {

    TrackNew::reset();
    IMap imap;
    imap.max_load_factor(5.0);
    imap.reserve(10); // heap allocation here

    EMap emap;

    auto [iter, status] = emap.emplace(1, std::move(imap) );
    auto& m = iter->second;
    m.emplace(1, 10);// heap allocation here

    TrackNew::status();
    return 0;
}

