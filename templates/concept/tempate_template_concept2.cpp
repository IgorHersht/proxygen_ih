//
// Created by igersht on 5/15/2024.
//

#include <iostream>

template<typename T> concept C = requires (T t) { t.f(); };
template<typename T> concept D = C<T> && requires (T t) { t.g(); };

template<template<C> class P> struct S { };

template<C> struct X { };
template<D> struct Y { };
template<typename T> struct Z { };
S<X> s1; // OK, X and P have equivalent constraints
//S<Y> s2; // error: P is not at least as specialized as Y
S<Z> s3; // OK, P is at least as specialized as Z

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}

