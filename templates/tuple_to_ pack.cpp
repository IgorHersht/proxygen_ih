#include <tuple>

// tuple to pack
template< typename... Ts >
struct C {
    std::tuple<Ts ...> m_args;
};
using Tp = std::tuple<int, char>;
//// tuple to pack
template< typename T > struct TupleToPack;
template<  typename... Ts > struct TupleToPack< std::tuple< Ts... > > { using Type = C< Ts... >; };
////
using C1 = C<TupleToPack<Tp>::Type>;

int main()
{
    C1 c1;
    int i =1;

    return 0;
}
