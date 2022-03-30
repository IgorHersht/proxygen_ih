#include <string>

template <int i>
struct a {
    static bool call (double d){
        return i < d;
    }
};

template <int i>
struct b {
    static int call (double d, const std::string& v){
        return i + d + std::stoi(v);
    }
};

template <int i>
struct c {
    static std::string call (double d, const std::string& v){
        return std::to_string(d) +  std::to_string(i) + v;
    }
};


template <template <int> class T, typename... Args>
decltype(auto) switchCall (int i, Args&&... args)
{
    switch(i){
        case 1: return T<1>::call(std::forward<Args>(args)...);
        case 2: return T<2>::call(std::forward<Args>(args)...);
    }
}

int main ()
{
    bool b1 = switchCall<a>(1, 3.3);
    int b2 = switchCall<b>(1, 3.3, "12");
    std::string b3 = switchCall<c>(1, 3.3, "12");

    int i = 1;
}

