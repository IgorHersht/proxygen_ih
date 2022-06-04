//
// Created by ihersht on 5/2/22.
//

#ifndef UNTITLED_GEN_STRING_H
#define UNTITLED_GEN_STRING_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <array>
#include <cstddef>  // for std::byte
#include <memory_resource>
#include <sys/mman.h>
#include <algorithm>
#include <iterator>
#include <random>
#include <sys/resource.h>

constexpr size_t IN_SIZE = 100000;
static std::vector<std::string> inputs(IN_SIZE);

static const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ-"
        "abcdefghijklmnopqrstuvwxyz";

template<typename URBG>
inline  std::string gen_string(std::size_t length, URBG&& g) {
    std::string result;
    result.resize(length);
    std::sample(std::cbegin(charset),
                std::cend(charset),
                std::begin(result),
                std::intptr_t(length),
                std::forward<URBG>(g));
    return result;
}

static std::mt19937 g;
inline void gen_strings(std::size_t length){
    for(std::string& in: inputs){
        in = gen_string(length, g).c_str();
    }
}


#endif //UNTITLED_GEN_STRING_H
