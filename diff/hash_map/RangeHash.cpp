#include <unordered_map>
#include <stddef.h>
#include <array>
#include <functional>

template< typename Range>  struct RangeHash {
    using value_type = typename Range::value_type;
    using iterator = typename Range::iterator;
    using const_iterator = typename Range::const_iterator;
    size_t operator()(const Range& range ) const {
        return hash(range.cbegin(), range.end());
    }
private:
    std::size_t hash(const_iterator first, const_iterator last) const {
        std::size_t seed = 0;

        for(; first != last; ++first) {
            combine(seed, *first);
        }

        return seed;
    }
     void combine(size_t & seed, value_type v) const {
        seed ^= std::hash<char>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
};


std::array ar1 {'a','b','b','d'};
std::array ar2 {'x','y','z', 'z'};

using Map = std::unordered_map<std::array<char, 4>, int, RangeHash<std::array<char, 4> > >;

int main()
{
    Map map;
    map.try_emplace(ar1, 1);
    map.try_emplace(ar2, 2);


    int i =1;
}
