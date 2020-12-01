#include <boost/thread.hpp>
#include <memory>
#include <iostream>

#include <shared_lib/library_shared.h>
#include <static_lib/library_static.h>

bool check(int i){
    return i >0;
}
int get() {
    return 5;
}

int test() {

    if (int j = get(); j < 5) {
        return -1;
    }
    else if(j == 5) {
        return 0;
    }
    else {
        return 1;
    }
}


int main()
{

    int i = test();
    if(int j = 5; check(j)){
      std::cout << j<< " true"  << std::endl;
    }else{
        std::cout << j<< " false"  << std::endl;
    }


    return 0;
}

// 5 true
