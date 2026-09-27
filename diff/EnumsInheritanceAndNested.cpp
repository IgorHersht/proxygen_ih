
#include <cstdint>
struct Colors
{
    using UnderlyingType = std::uint8_t;
    struct Basic {
        enum : UnderlyingType
        {
            Red,
            Green,
        };
        static constexpr UnderlyingType last{ Green };
    };

    struct Extended : Basic {
        enum : UnderlyingType
        {
            Blue = Basic::last + 1,
            Red,
        };
        static constexpr UnderlyingType last{ Red };
    };

    struct Other
    {
        enum : UnderlyingType
        {
            Orange = Extended::last + 1,
            Purple,
            Yellow
        };
        static constexpr UnderlyingType last{ Yellow };
    };
    static constexpr UnderlyingType size{ Other::last + 1 };
};


#include <type_traits>
static_assert(Colors::Basic::Red == 0);
static_assert(Colors::Basic::Green == 1);
static_assert(std::is_same_v<decltype(Colors::Basic::Green), decltype(Colors::Basic::Red)>); // the same types

static_assert(Colors::Extended::Blue == 2);
static_assert(!std::is_same_v<decltype(Colors::Basic::Green), decltype(Colors::Extended::Blue)>); // not the same types

static_assert(Colors::Extended::Green == 1); // inheritance use base
static_assert(Colors::Extended::Red == 3); // inheritance  overridden

static_assert(Colors::Other::Orange == 4);
static_assert(Colors::Other::Purple == 5);
static_assert(Colors::Other::Yellow == 6);
static_assert(!std::is_same_v<decltype(Colors::Basic::Green), decltype(Colors::Other::Orange)>); // not the same types

static_assert(Colors::size == 7);

#include <magic_enum/magic_enum.hpp>
#include <string_view>

constexpr std::string_view red_test{ "Red" };
constexpr std::string_view red1 = magic_enum::enum_name(Colors::Basic::Red);
constexpr std::string_view red2 = magic_enum::enum_name(Colors::Extended::Red);
static_assert(red_test == red1);
static_assert(red_test == red2);
static_assert(!std::is_same_v<decltype(Colors::Basic::Red), decltype(Colors::Extended::Red)>); // not the same types

constexpr size_t basicSize{ magic_enum::enum_count<decltype(Colors::Basic::Red)>() };
static_assert(basicSize == 2);

static_assert(magic_enum::enum_value<decltype(Colors::Basic::Red)>(1) == Colors::Basic::Green);

int main() {

}



