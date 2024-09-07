#include <string>
#include <iostream>
#include <set>
template<typename T, typename ...E> void  multi_insert(T &t, E&&...e){
    // unary right fold over comma
    (void(t.insert(std::forward<E>(e))), ...);
}

int main() {
    std::set<int> s;
    multi_insert(s, 1, 4, 7, 10);
    for (auto i : s)
        std::cout << i << " ";
    std::cout << std::endl;
    // prints:
    // 1 4 7 10
}

