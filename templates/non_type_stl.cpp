
#include <array>
#include <string_view>
#include <tuple>
// think how to prevent the object creation in non-constexpr context
template<std::size_t Size> struct NonTypeStr {
    std::array<char, Size> str{};
    consteval auto operator<=>(const NonTypeStr &) const = default;
    consteval std::string_view value() const {
        return str.data();
    }
};
template<std::size_t Size> NonTypeStr(const char(&)[Size] ) -> NonTypeStr<Size>; // deduction guide


//test
template<auto Prefix> struct Logger {
    inline static constexpr auto prefix = Prefix;
};
static_assert( Logger<NonTypeStr{"xxx"}>::prefix == NonTypeStr{"xxx"});
static_assert( Logger<NonTypeStr{"xxx"}>::prefix.value() == "xxx");
static_assert( std::is_same_v<Logger<NonTypeStr{"xxx"}>, Logger<NonTypeStr{"xxx"}>>);
static_assert( !std::is_same_v<Logger<NonTypeStr{"xxx"}>, Logger<NonTypeStr{"yyy"}>>);

static_assert( std::is_same_v<Logger<std::array{2,5, 7}>, Logger<std::array{2,5, 7}>>);
static_assert( !std::is_same_v<Logger<std::array{2,5, 7}>, Logger<std::array{2,6, 7}>>);
static_assert( !std::is_same_v<Logger<std::array{2,5, 7}>, Logger<std::array{2,5, 7, 8}>>);

static_assert( std::is_same_v<Logger<std::pair{2,5.0}>, Logger<std::pair{2,5.0}>>);
static_assert( !std::is_same_v<Logger<std::pair{2,5.0}>, Logger<std::pair{3,5.0}>>);
static_assert( !std::is_same_v<Logger<std::pair{2,5.0}>, Logger<std::pair{2,5}>>);

//tuple does not work
// "is not a valid type for a template non-type parameter because it is not structural
// static_assert( std::is_same_v<Logger<std::tuple{2,5.0}>, Logger<std::tuple{2,5.0}>>);
// static_assert( !std::is_same_v<Logger<std::tuple{2,5.0}>, Logger<std::tuple{3,5.0}>>);
// static_assert( !std::is_same_v<Logger<std::tuple{2,5.0}>, Logger<std::tuple{2,5}>>);

int  main() {
}
