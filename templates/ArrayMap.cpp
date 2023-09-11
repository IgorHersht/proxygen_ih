#include <array>
#include <string.h>

template<typename T, size_t SIZE > struct ArrayMap {
    using Key = size_t;
    using Type = T;
    inline static T Default{};
    static constexpr size_t Size = SIZE;
    static constexpr Key Begin = 0;
    static constexpr Key End = SIZE;

    template< class... Args > std::pair<Key, bool> try_emplace( Key key, Args&&... args ) {
        if( key >= Size) [[unlikely]]{
            return {End, false};
        }
        if(_map[key] != Default ) {
          return {key, false};
        }
        _map[key] = Type{std::forward<Args>(args) ...};
        return {key, true};
    }

    template< class M >
    std::pair<Key, bool> insert_or_assign(Key key, M&& obj ) {
        if( key >= Size) [[unlikely]]{
            return {End, false};
        }
        _map[key] = Type{std::forward<M>(obj)};
        return {key, true};
    }

    Type* find(Key key) {
        if( key >= Size) [[unlikely]]{
            return nullptr;
        }
        if(_map[key] == Default) {
            return nullptr;
        }
        return &_map[key];
    }

    const Type* find(Key key) const {
        if( key >= Size) [[unlikely]]{
            return nullptr;
        }
        if(_map[key] == Default) {
            return nullptr;
        }
        return &_map[key];
    }

    Key findValue( const Type& value) const{
        for(Key key = Begin; key != End; ++key  ) {
            if(value == _map[key]){
                return key;
            }
        }
        return End;
    }

    bool empty() const {
       return _map == EmptyMap;
    }
    void clear() {
        memcpy(&_map, &EmptyMap, SIZE);
    }
private:
    using Map = std::array<T, SIZE>;
    inline static Map EmptyMap{};
    Map _map{EmptyMap};
};

///// test
#include <iostream>
#include <assert.h>
void test0 (){
    {
        ArrayMap<size_t, 20> map;
        assert(map.empty());

        size_t v1{10}; size_t v2{20};  size_t v3{20};
        auto[p1,s1] = map.try_emplace(1, v1);
        assert(p1 = 1);
        assert(s1);
        auto[p2,s2] = map.try_emplace(2, v2);
        assert(p2 = 2);
        assert(s2);
        auto[p3,s3] =map.try_emplace(1, v3);
        assert(p3 = p1);
        assert(!s3);
        assert(*map.find(1) == v1);
        assert(*map.find(2) == v2);
        assert(map.find(5) == nullptr);
        map.clear();
        assert(map.empty());
        {
            auto [p1, s1] = map.insert_or_assign(1, v1);
            assert(p1 = 1);
            assert(s1);
            auto [p2, s2] = map.insert_or_assign(2, v2);
            assert(p2 = 2);
            assert(s2);
            auto [p3, s3] = map.insert_or_assign(1, v3);
            assert(p3 = p1);
            assert(s3);
            assert(*map.find(1) == v3);
            assert(*map.find(2) == v2);
        }

    }




};

int main()
{
    test0();

}

