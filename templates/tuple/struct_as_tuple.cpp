#include <tuple>
#include <concepts>
#include <array>
#include <cassert>

template<class T>
concept class_type= std::is_class_v<T>;

struct any_type {
    template<class T> operator T() {}
};

template<class_type T>
consteval std::size_t count_members(auto ...members) {
    if constexpr (!requires { T{ members... }; })
        return sizeof...(members) - 1;
    else
        return count_members<T>(members..., any_type{});
}
// MAYDO version to move data from srtuct
template<class_type T> constexpr auto struct_as_tuple(const T& data) {
    constexpr std::size_t fieldCount = count_members<T>();

    //Nasty but Structured Bindings does not have a Pack now
    if constexpr(fieldCount == 0) {
        return std::tuple();
    } else if constexpr (fieldCount == 1) {
        const auto& [m1] = data;
        return std::tuple(m1);
    } else if constexpr (fieldCount == 2) {
        const auto& [m1, m2] = data;
        return std::tuple(m1, m2);
    } else if constexpr (fieldCount == 3) {
        const auto& [m1, m2, m3] = data;
        return std::tuple(m1, m2, m3);
    } else if constexpr (fieldCount == 4) {
        const auto& [m1, m2, m3, m4] = data;
        return std::tuple(m1, m2, m3, m4);
    } else if constexpr (fieldCount == 5) {
        const auto& [m1, m2, m3, m4, m5] = data;
        return std::tuple(m1, m2, m3, m4, m5);
    }else if constexpr (fieldCount == 6) {
        const auto& [m1, m2, m3, m4, m5, m6] = data;
        return std::tuple(m1, m2, m3, m4, m5, m6);
    }else if constexpr (fieldCount == 7) {
        const auto& [m1, m2, m3, m4, m5, m6, m7] = data;
        return std::tuple(m1, m2, m3, m4, m5, m6, m7);
    }else if constexpr (fieldCount == 8) {
        const auto& [m1, m2, m3, m4, m5, m6, m7, m8] = data;
        return std::tuple(m1, m2, m3, m4, m5, m6, m7, m8);
    }else {
        static_assert(fieldCount!=fieldCount, "Too many fields for struct_as_tuple(...)! add more if statements!");
    }
}

// test
struct C{};
struct St{ int i{1}; char ch{'c'}; std::array<int,5> ar{25}; C c;  };

int main() {
    //Types test
    using Tuple = decltype(struct_as_tuple(std::declval<St>()));
    static_assert(std::same_as<int, std::tuple_element_t<0, Tuple>>);
    static_assert(std::same_as<char, std::tuple_element_t<1, Tuple>>);
    static_assert(std::same_as<std::array<int,5>, std::tuple_element_t<2, Tuple>>);
    static_assert(std::same_as<C, std::tuple_element_t<3, Tuple>>);

    //Values test
    constexpr St st;
    constexpr Tuple tuple = struct_as_tuple(st);
    static_assert(st.ch == std::get<1>(tuple) );

    St st2;
    Tuple tuple2 = struct_as_tuple(st);
    assert(st2.ch == std::get<1>(tuple2) );
}

