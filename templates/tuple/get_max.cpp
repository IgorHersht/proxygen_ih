template <typename... U>
struct IsTupleImpl<std::tuple <U...>> : std::true_type {};

template <typename T> constexpr static bool is_tuple_v = IsTupleImpl<std::decay_t<T>>::value;


template <class... Ts>  consteval size_t get_max_size() requires (sizeof...(Ts) > 1)  { return std::max({sizeof(Ts)...}); }
template <class T>  consteval size_t get_max_size()   { return sizeof(T); }

template <class... Ts>  consteval size_t _get_max_size(std::tuple<Ts...>*  )  { return get_max_size<Ts...>(); }
template <class T> static consteval size_t get_max_size( ) requires (is_tuple_v<T>) {
    if constexpr (std::tuple_size_v<T> > 1){
        return _get_max_size((T*) nullptr);
    }
    return sizeof(std::tuple_element_t<0,T>);
}



//test

int main() {
    struct V {
        std::array<char, 16> ar;
    };

   static_assert(16 == get_max_size<char, int, V, double>());
   static_assert(4 == get_max_size<int>());
   static_assert(16 == get_max_size <std::tuple<char, int, V, double>>());
   static_assert(8 == get_max_size<std::tuple<double>>());
}
