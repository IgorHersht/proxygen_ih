//https://stackoverflow.com/questions/8569567/get-part-of-stdtuple


#include <utility>
#include <tuple>

namespace detail
{
    template <std::size_t Ofst, class Tuple, std::size_t... I> consteval auto slice_impl(Tuple&& t, std::index_sequence<I...>) {
        return std::make_tuple( std::get<I + Ofst>(std::forward<Tuple>(t))...);
    }
    template <std::size_t begin, std::size_t end, class Tuple> consteval auto tuple_slice(Tuple&& t) {
        static_assert(end >= begin, "invalid slice");
        static_assert(std::tuple_size_v<std::decay_t<Tuple>> >= end,"slice index out of bounds");
        auto tuple = slice_impl<begin>(std::forward<Tuple>(t), std::make_index_sequence<end - begin>{});
        return std::decay_t<decltype(tuple)>(tuple);
    }
}
template <std::size_t begin, std::size_t end, class Tuple> static constexpr auto tuple_slice_v = detail::tuple_slice<begin, end>(Tuple{});


namespace detail
{
    template <typename ET, std::size_t pos, class Tuple> consteval auto insert_in_tuple(Tuple&& t) {
        constexpr auto tail = tuple_slice_v<0, pos,Tuple>;
        constexpr auto head = tuple_slice_v<pos, std::tuple_size_v<Tuple>, Tuple>;
        return std::tuple_cat(tail, std::make_tuple(ET()), head );
    }
}
template <typename ET, std::size_t pos, class Tuple>  static constexpr auto insert_in_tuple_v = detail::insert_in_tuple<ET, pos>(Tuple{});

class C{};

// test
using Tuple = std::tuple<int, long ,char, short, short >;
constexpr auto tail = tuple_slice_v<0, 2,Tuple>;
constexpr auto head = tuple_slice_v<2, std::tuple_size_v<Tuple>, Tuple>;
static_assert(std::is_same_v< decltype(tail), const std::tuple<int, long> >);
static_assert(std::is_same_v< decltype(head), const std::tuple<char, short, short> >);
constexpr auto newTuple = insert_in_tuple_v<C, 3, Tuple>;
static_assert(std::is_same_v< decltype(newTuple), const std::tuple<int, long ,char,  C, short, short > >);

int  main() {
   
}
