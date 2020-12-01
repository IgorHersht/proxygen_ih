
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
#include<iterator>
#include<string>





int main(int argc, char* argv[]){
 std::vector<int> v{2,4,5,6,8,7,4};
 auto it= std::find(v.cbegin(), v.cend(),4 );  
 v.insert(it, 11);
    
 return 0;
}
 
