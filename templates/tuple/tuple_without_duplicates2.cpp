#include <iostream>
#include <tuple>

template < typename T, typename ...Rest >
constexpr auto make_unique_tuple( std::tuple< T, Rest... > )
{
    if constexpr ( ( std::is_same_v< T, Rest > || ... ) )
    {
        return make_unique_tuple( std::tuple< Rest... >{} );
    }
    else
    {
        if constexpr ( sizeof...( Rest ) > 0 )
        {
            using remaining = decltype( make_unique_tuple( std::tuple< Rest... >{} ) );
            return std::tuple_cat( std::tuple< T >{}, remaining{} );
        }
        else
        {
            return std::tuple< T >{};
        }
    }
}

template <typename T> void TD() { std::cout << __PRETTY_FUNCTION__ << std::endl;  }

int main()
{
    TD< decltype( make_unique_tuple( std::tuple< int, char, int, char, float, char, double >{} ) ) >();
}

