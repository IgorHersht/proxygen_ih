//
// Created by root on 6/8/22.
//

#ifndef HASH_MAP_MONOTONICINTEGRALHASHMAP_H
#define HASH_MAP_MONOTONICINTEGRALHASHMAP_H

#include <unordered_map>
#include <memory>
#include <cstddef>  // for std::byte
#include <memory_resource>
#include <sys/mman.h>

#ifndef uint128_t
# define uint128_t  unsigned __int128
#endif

//from Google's cityhash
constexpr uint64_t hash_128_to_64( uint64_t upper, uint64_t lower) noexcept {
    const uint64_t kMul = 0x9ddfea08eb382d69ULL;
    uint64_t a = (lower ^ upper) * kMul;
    a ^= (a >> 47);
    uint64_t b = (upper ^ a) * kMul;
    b ^= (b >> 47);
    b *= kMul;
    return b;
}

struct Uint128Hasher {
    size_t operator()(uint128_t u) const noexcept {
        auto const hi = static_cast<uint64_t>(u >> 64);
        auto const lo = static_cast<uint64_t>(u);
        return hash_128_to_64(hi, lo);
    }
};

template< typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key> > class MonotonicIntegralHashMap{
    static_assert( ( (std::is_integral_v<Key>  || std::is_enum_v<Key>  ) &&  std::is_unsigned_v<Key> ) || std::is_same_v<Key, unsigned char> || std::is_same_v<Key, uint128_t>, "Should be a unsigned int type");
    constexpr static size_t element_size =  sizeof(std::pmr::unordered_map< Key, T >::value_type ) + 2*sizeof(void*) + alignof(std::max_align_t); //pool size estimate per map element
    constexpr static size_t loadFactor = 5;
public:
    using Map = std::pmr::unordered_map<Key, T>;
    using iterator = typename Map::iterator;
    using const_iterator = typename Map::const_iterator;

    explicit MonotonicIntegralHashMap(size_t bucket_count):
        _bufferSize(bucket_count * loadFactor * loadFactor),
        _buffer(new std::byte[_bufferSize]),
        _pool(_buffer.get(), _bufferSize, std::pmr::new_delete_resource()),
        _map(&_pool) {
                _map.max_load_factor(_map.max_load_factor() * loadFactor);
                mlock(_buffer.get(), _bufferSize);
        }
    bool contains ( const Key& key ) const{
        return _map.contains();
    };

    iterator find( const Key& key ){
        return _map.find();
    }

    template< class... Args >
    std::pair<iterator,bool> tryEmplace( Args&&... args){
        return _map.try_emplace(std::forward<Args>(args)...);
    }

    std::pair<iterator, bool> insertOrAssign( const Key& key, T&& value ){
        return _map.insert_or_assign(key, std::forward<T>(value));
    }

    iterator erase( iterator pos ){
        return _map.erase(pos);
    }

    size_t erase( const Key& key ){
        return _map.erase(key);
    }

private:
    size_t    _bufferSize;
    std::unique_ptr<std::byte[]> _buffer;
    std::pmr::monotonic_buffer_resource _pool;
    std::pmr::unordered_map<Key, T, Hash, KeyEqual > _map;
};

#endif //HASH_MAP_MONOTONICINTEGRALHASHMAP_H
