#include <tuple>
#include <utility>
#include <array>
#include <type_traits>
#include <string.h>
#include <string>
#include<assert.h>
#include "tracknew.h"
using MT = std::tuple<  double, char,
        std::array<char,64>, std::array<char,64>, std::array<char,64>, std::array<char,64>, std::array<char,64>, std::array<char,64>, std::array<char,64>, std::array<char,64>,std::array<char,64>, std::array<char,64>,
        std::array<char,64>, std::array<char,64>, std::array<char,64>, std::array<char,64>, std::array<char,64>, std::array<char,64>, std::array<char,64>, std::array<char,64>,std::array<char,64>, std::array<char,64>,
        std::array<char,64>, std::array<char,64>, std::array<char,64>, std::array<char,64>, std::array<char,64>, std::array<char,64>, std::array<char,64>, std::array<char,64>,std::array<char,64>, std::array<char,64>
        >;

//std::array<std::byte, sizeof(MT)> ar{'1'};
std::array<char,64> ar0 {};
std::array<char,64> ar1{1, 2, 4, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30  };
MT mt0{0.0, 0, ar0, ar0,ar0,ar0,ar0,ar0,ar0,ar0,ar0,ar0,
       ar0,ar0,ar0,ar0,ar0,ar0,ar0,ar0,ar0,ar0,
       ar0,ar0,ar0,ar0,ar0,ar0,ar0,ar0,ar0,ar0};

MT mt1{1.1, 'X', ar1, ar1,ar1,ar1,ar1,ar1,ar1,ar1,ar1,ar1,
       ar1,ar1,ar1,ar1,ar1,ar1,ar1,ar1,ar1,ar1,
       ar1,ar1,ar1,ar1,ar1,ar1,ar1,ar1,ar1,ar1};


struct D{
    D() = default;
    D(MT mt):_mt(mt) {
    }
    auto operator <=> (const D&) const = default;
    MT _mt{mt0};
};

int main()
{
    std::array<std::byte, sizeof(MT)> data {};
    TrackNew::reset();
    {
        D d1(mt1);
        memcpy(&data,  &d1, sizeof(MT));
        d1 = D();
        int i =1;
    }
    TrackNew::status();
    D* dp = new D;
    memcpy(dp, &data, sizeof(MT));
    D d11(mt1);
    bool st = (*dp == d11);
    assert(st);
    int i =1;

}
