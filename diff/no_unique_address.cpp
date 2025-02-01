#include <assert.h>
#include <tuple>

template <typename T1, typename T2> struct C {
    [[no_unique_address]] T1 t1;
    [[no_unique_address]] T2 t2;
};

struct Empty{};

int main() {
    static_assert(sizeof(C<int, Empty>) == 4);
    C<int, Empty> c1;
    assert((void*)&c1.t1 == (void*)&c1.t2);
    static_assert(sizeof(std::tuple<int, Empty>) == 4);
}
