#include <tuple>

template <typename ... Ts>
constexpr std::size_t TupleSumSize (std::tuple<Ts...> const &)
{ return ( sizeof(Ts) + ... ); }

template  <typename IdT, template<IdT> typename ElementT,  IdT ... ids> struct IdsTupleTrait {
    using Tuple = std::tuple< ElementT<ids> ... >;
    static constexpr Tuple _default{};
};
/// test
#include <cstdint>
struct Ids {
    using Type = int16_t;
    static constexpr Type unknown = 0;
    static constexpr Type id1 = 1;
    static constexpr Type id2 = 2;
    static constexpr Type id3 = 3;
    static constexpr Type id4 = 4;
    static constexpr Type id5 = 5;
    static constexpr Type id6 = 6;
};



template <Ids::Type is> struct St;

template <> struct St<Ids::id1> {
    using Type = char;
};

template <> struct St<Ids::id3> {
    using Type = int64_t;
};

template <> struct St<Ids::id5> {
    using Type = int32_t;
};

using MyTupleTrait = IdsTupleTrait<Ids::Type, St,  Ids::id1, Ids::id3, Ids::id5>;
using MyTuple = MyTupleTrait::Tuple;


int main()
{
    MyTuple my;
    int i =1;
}
