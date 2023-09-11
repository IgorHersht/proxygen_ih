#include <bitset>
#include <limits>

struct Flags {
    using Type = u_int8_t;
    enum Check : Type {  CapLimit = 1, PriceRange = 2, CreditLimit = 4 };
    constexpr Flags() = default;
    constexpr Flags(Check check): _checks(Type(check)) {
    }
    void set(Check check) {
        _checks |= Type(check);
    }
     void unSet(Check check) {
        _checks &= ~Type(check);
    }

    void setAll() {
        _checks = ALL;
    }

     void setNone() {
        _checks = NONE;
    }

    constexpr bool isSet(Check check) const{
        return (Type(check) & _checks);
    }

    constexpr bool isNone() const {
       return  (_checks == NONE);
    }

    constexpr bool isAall() const {
        return  (_checks == ALL);
    }

private:
    static constexpr Type NONE{};
    static constexpr Type ALL{std::numeric_limits<Type>::max()};
    Type  _checks{ALL};// default set all
};

int main()
{
    Flags chs{};
    bool s0 = chs.isNone();
    chs.unSet(Flags::CapLimit);
    chs.unSet(Flags::PriceRange);
    chs.unSet(Flags::PriceRange);
    chs.unSet(Flags::CreditLimit);
    bool s1 = chs.isSet(Flags::CapLimit);
     bool s2 = chs.isSet(Flags::PriceRange);
    bool s3 = chs.isSet(Flags:: CreditLimit);
    chs.setNone();
    bool s4 = chs.isNone();

    chs.setNone();
    bool s6 = chs.isNone();
    chs.setAll();
    int i =1;

    return 0;
}
