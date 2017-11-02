#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <boost/unordered_map.hpp>
#include <boost/lexical_cast.hpp>



// Perfect hash

struct ihash{
    std::size_t operator()(std::size_t i) const{
        return i;
    }
};
typedef boost::unordered_map<int, const char*,  ihash > MapT;

int main()
{

//////////////////
	MapT map1(53);
	const char* s1 = "xxxxx";
	const char* s2 = "yyyyy";
	map1[25] = s1; map1[10] = s2;
	for(MapT::iterator v = map1.begin(), end = map1.end(); v != end; ++v) {
	  std::cout <<"bucket #=" << v->first<<"; bucket_size="<< map1.bucket_size(v->first);
	  if(v->second) {
		std::cout <<"; value="<< v->second;
	  }
		 std::cout << std::endl;
	}

  return 0;
}

