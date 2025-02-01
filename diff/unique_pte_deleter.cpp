
#include <cxxabi.h>
#include <type_traits>
#include <memory>
#include <string>

auto deleter1 = [] (int* p) { delete p; };
auto deleter2 = [] (int* p) { delete p; };

std::unique_ptr<int, decltype(deleter1)> ptr1(new int(43), deleter1);
std::unique_ptr<int, decltype(deleter2)> ptr2(new int(42), deleter2);
std::unique_ptr<int, decltype(deleter2)> ptr3(new int(42), deleter2);

using Ptr = std::unique_ptr<int, decltype(deleter1)>;

int main() {
    
   static_assert(!std::is_same_v<decltype(ptr1), decltype(ptr2)>);
    static_assert(std::is_same_v<decltype(ptr2), decltype(ptr3)>);

    return 0;
}
