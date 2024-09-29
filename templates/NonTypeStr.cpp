#include <array>
#include <string_view>
// think how to prevent the object creation in non-constexpr context
template<std::size_t Size> struct NonTypeStr {
    std::array<char, Size> str{};
    auto operator<=>(const NonTypeStr &) const = default;
    consteval std::string_view value() const {
        return str.data();
    }
};
template<std::size_t Size> NonTypeStr(const char(&)[Size] ) -> NonTypeStr<Size>; // deduction guide

//test
template<auto Prefix> struct Logger {
    inline static constexpr auto prefix = Prefix;
};
static_assert( !std::is_same_v<Logger<NonTypeStr{"xxx"}>, Logger<NonTypeStr{"yyy"}>>);
static_assert( Logger<NonTypeStr{"xxx"}>::prefix == NonTypeStr{"xxx"});
static_assert( Logger<NonTypeStr{"xxx"}>::prefix.value() == "xxx");

int  main() {
}
