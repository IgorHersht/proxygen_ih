//
// Created by ihersht on 9/24/21.
//

#pragma once

#include<string>
#include <iostream>
#include <mutex>
// tmp code. Will be replaced with
static std::mutex ioMutex;

inline void log(const std::string& msg){
    std::lock_guard<std::mutex> lc(ioMutex);
    std::cout << msg << std::endl;
}
