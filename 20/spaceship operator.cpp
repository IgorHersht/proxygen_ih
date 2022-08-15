#include <map>
#include <assert.h>
#include<string>
#include <array>
#include <compare>

template <typename T1, typename T2 >
std::partial_ordering getPartialOrdering( const T1& v1, const T2& v2 ) {
    return v1 <=>v2;
}

struct C{
    C(int v):_v(v){}
    friend auto operator<=>(const C& c, int v){
       return c._v <=> v;
    }
    int _v;
};

std::array ar1{1,2};
std::array ar2{1,3};

int main(){

    auto c0 = (2 <=> 3);// std::strong_ordering
    auto c1 = (void*)2 <=> (void*)3;// std::strong_ordering
    auto c2 = std::string ("HELLO") <=> std::string ("hello"); //std::strong_ordering
    auto c3 = std::string_view ("HELLO") <=> std::string_view ("hello");// std::strong_ordering
    auto c5  = 2 <=> C(5);// std::strong_ordering
    auto c6 = ar1 <=> ar2; // std::strong_ordering

    auto c10 = 2.3 <=>2.0;// std::partial_ordering

    int i = 1;
    
}

