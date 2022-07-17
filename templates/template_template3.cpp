#include <tuple>
#include <variant>


template< template <int> class T, template < typename ...> class CollectionT>
using TopicCollection = CollectionT< T<1>, T<2>, T<3> >;

template< template <int> class T>
using TopicTuple = TopicCollection< T, std::tuple>;

template< template <int> class T>
using TopicVariant = TopicCollection< T, std::variant>;


/// test - usage
template <int i> struct C {
    int j = i;
};

int main()
{
    TopicCollection<C, std::tuple> t1;
    TopicTuple<C> t2;
    TopicCollection<C, std::variant> v1;
    TopicVariant<C> v2;

    int i = 1;
    return 0;
}

