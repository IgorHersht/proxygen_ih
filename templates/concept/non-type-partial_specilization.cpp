
#include <tuple>
#include <array>
#include <memory>
struct C {
    char ch = 'x';
    std::array<int,5> ar{3, 7, 25, 4, 88 };
    char str[6] = {'a', 'b', 'c', '\0'};
    double b{0.45};
};


using MyTuple = std::tuple< char, std::array<int,5>,  char [6], double>;


void setTuple() {
    //static_assert(sizeof(MyTuple)== sizeof(C));
}



int main() {
    static_assert(sizeof(MyTuple)== sizeof(C));
int i = sizeof(MyTuple);
    int j = sizeof(C);
    int x =1;



}

