#include <iostream>
#include <type_traits>
#include <string>
#include <tuple>
#include <variant>
#include <assert.h>

template<typename T> concept C1 = requires(T t) { --t; };
template<typename T> concept C2 = C1<T> && requires(T t) { *t; };
// function specilization
template<C1 T> int f(T) { return 1; } // #1
template<C2 T> int f(T) { return 2; }// #2
// function specilization
template<typename T> int f1(T) requires C1<T> { return 1; } // #1
template<typename  T> int f1(T) requires C2<T> { return 2; }// #2
// function specilization
template<typename T> requires C1<T> int f2(T)  { return 1; } // #1
template<typename  T> requires C2<T> int f2(T)  { return 2; }// #2
// function specilization
int g(C1 auto c1) { return 1; } // #1
int g(C2 auto c2) { return 2; }// #2
//

// member function specilization
template<typename T> struct S {
    int f() requires C1<T> { return 1; } // #1
    int f() requires C2<T> { return 2; } // #2
};
// class partial specilization
int r = -1;
template<typename T> struct X {  X() { r = 0;} }; // #0
template<C1 T> struct X<T> {  X() { r = 1;} }; // #1
template<C2 T> struct X<T> {  X() { r = 2;} }; // #1;


int main() {
    // function specilization
    assert(1 == f(0)); // call #1
    assert(2 == f((int*)0)); // call #2
    assert(1 == f1(0)); // call #1
    assert(2 == f1((int*)0)); // call #2
    assert(1 == f2(0)); // call #1
    assert(2 == f2((int*)0)); // call #2
    assert(1 == g(0)); // call #1
    assert(2 == g((int*)0)); // call #2
    // member function specilization
    S<int> s1; S<int*> s2;
    assert(1 == s1.f()); // call #1
    assert(2 == s2.f()); // call #2
    // class partial specilization
    X<bool> x0;  assert(r == 0); //#0 pramary template
    X<int> x1;  assert(r == 1); //#1 C1 specilization
    X<int*> x2;  assert(r == 2); //#2 C2 specilization
}

