
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>
#include <iostream>
#include <exception>
#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<list>
#include<vector>
#include<queue>
#include<algorithm>
#include "boost/multi_array.hpp"

 typedef boost::multi_array<double, 3> array_type;
 array_type A(boost::extents[3][4] [2]);
 double ar [3][4] [2];


int main(int argc, char* argv[])
{
    int s1 = sizeof(A);// 160 ??
    int s2 = sizeof(ar); // 192
 return 0;
}
 
