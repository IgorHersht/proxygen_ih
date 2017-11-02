
#include <utility>
#include <iostream>
#include <memory>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <string.h>
#include <unordered_map>


struct ihash{
    std::size_t operator()(std::size_t i) const{
        return i;
    }
};

std::unordered_map<int, const char*,  ihash > map1(53);

char c1='x'; char c2='y';

const char* s1 = "xxxxx";
const char* s2 = "yyyyy";
int main(int argc, char* argv[]){
std::size_t sz = map1.bucket_count();
map1[25] = s1; map1[10] = s2; 


for(auto v: map1) {
  std::cout <<"bucket #=" << v.first<<"; bucket_size="<< map1.bucket_size(v.first);
  if(v.second) {
    std::cout <<"; value="<< v.second;
  }
     std::cout << std::endl;
}

  return 0;
}
/*
bucket #=10; bucket_size=1; value=yyyyy
bucket #=25; bucket_size=1; value=xxxxx
 
*/
