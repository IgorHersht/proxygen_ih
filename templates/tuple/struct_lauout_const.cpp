#include <iostream>
#include <array>

template<typename Class, typename MemberType>
consteval std::size_t getOffset(MemberType Class::* member) {
    union u {
        constexpr u(){}
        char a[sizeof(Class)]{};
        Class t;
    } x;
    auto* p =  &(x.t.*member);
    for (std::size_t i = 0; i < sizeof(Class) ; ++i) {
        if (static_cast<void*>(x.a + i) == p) {
            return i;
        }
    }
    return sizeof(Class);
}

// test

struct  MyClass {
    struct C {
        char ch;
        std::array<double,5> d;
    };
   // MyClass() {}
    int a;
    char ch;
    std::array<float,5> f;
    C c;
    double d;
};

int main() {
    constexpr size_t of2 = getOffset<MyClass>(&MyClass::f);
    static_assert(getOffset<MyClass>(&MyClass::a) == offsetof(MyClass, a));
    static_assert(getOffset<MyClass>(&MyClass::ch) == offsetof(MyClass, ch));
    static_assert(getOffset<MyClass>(&MyClass::f) == offsetof(MyClass, f));
    static_assert(getOffset<MyClass>(&MyClass::c) == offsetof(MyClass, c));
    static_assert(getOffset<MyClass>(&MyClass::d) == offsetof(MyClass, d));


    return 0;
}
