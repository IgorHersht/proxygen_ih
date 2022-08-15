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

constexpr size_t IN_SIZE = 10000000;
static std::vector<std::string> inputs(IN_SIZE);

static const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
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
inline void gen_strings(){
    for(std::string& in: inputs){
        in = gen_string(20, g);
    }
}

using Pagefaults = std::pair<int,int>;
inline Pagefaults newPagefaultCount(){
    static int last_majflt = 0, last_minflt = 0;
    rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    std::pair<int,int> pagefauls{usage.ru_majflt - last_majflt,usage.ru_minflt - last_minflt };
    last_majflt = usage.ru_majflt;
    last_minflt = usage.ru_minflt;

    return pagefauls;
}

void print(Pagefaults pagefaults){
    std::cout<< "Major-Pagefaults="<< pagefaults.first << " Minor-Pagefaults=" << pagefaults.second << std::endl;
}

#endif //UNTITLED_GEN_STRING_H
