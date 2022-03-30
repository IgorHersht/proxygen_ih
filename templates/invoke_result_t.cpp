#include <type_traits>
template<class> inline constexpr bool always_false_v = false;

template <typename F > void foo(F&& f ){
    if constexpr(std::is_integral_v<std::invoke_result_t<F, int> >) {
        int j = 1;
    } else if constexpr(std::is_void_v<std::invoke_result_t<F,int> >) {
        int j = 1;
    } else  {
        static_assert( always_false_v<F>, "foo cannot be instatoeted with this type");
    }

}

void f1( int i){
    int j =i;
}

int f2( int i){
   return i;
}

int main(){
    foo(f1);
    foo(f2);

    return 0;
}



