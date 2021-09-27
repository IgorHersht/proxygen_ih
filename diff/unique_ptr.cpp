
#include <utility>
#include <iostream>
#include <memory>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <string.h>

using UPS = std::unique_ptr<std::string>;
std::vector<UPS> vsp;


int main(int argc, char* argv[]){

  for (int i = 0; i < 100; ++i) {
vsp.push_back(UPS(new std::string("Hello")));}

std::sort(vsp.begin(), vsp.end(),
[](const UPS& p1, const UPS& p2)
{ return *p1 < *p2; });

  return 0;
}
/*
name=igor
name=igor
 
*/
