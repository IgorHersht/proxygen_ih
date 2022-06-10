#include "IntegralValue.h"

//test
#include <assert.h>
#include <iostream>
#include <set>
#include "MonotonicIntegralHashMap.h"
#include "gen_string.h"



void testSplit(){
    AlphaNumericIntegralValue v("zzzuhug3rbb4bhj4bbxx");
    assert(v.isValid());
    uint128_t var =v;
    uint64_t upper1 = static_cast<uint64_t>(var >> 64);
    uint64_t lower1 = static_cast<uint64_t>(var);;
    uint64_t upper2 =0;
    uint64_t lower2 = 0;
    memcpy(&upper2, (char*)(&var) +8, 8 );
    memcpy(&lower2, &var, 8 );
    assert(upper1 == upper2);
    assert(lower1 == lower2);
}

void testMap(){
    MonotonicIntegralHashMap<uint128_t, uint128_t,Uint128Hasher> map(IN_SIZE);
    gen_strings(19);
    inputs.emplace_back(inputs[0]); // to have dup
    std::multiset<uint128_t> ints;
    for(std::string& in: inputs){
        ints.emplace(AlphaNumericIntegralValue(in));
    }

    size_t p = 0;
    for(uint128_t e: ints ){
        bool unique = map.tryEmplace(e, p).second;
        if(!unique){
            assert( ints.count(e) > 1 ) ;
        }
        ++p;
    }
}

int main() {
    testSplit();
    testMap();



    return 0;
}

