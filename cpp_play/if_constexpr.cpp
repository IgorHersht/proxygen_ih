#include <type_traits>
#include <iostream>


template<typename T> void foo(T t) {

  if constexpr(std::is_integral_v<T>) {
    std::cout << "int" << std::endl;
  } else if constexpr(std::is_floating_point_v<T>) {
    std::cout << "float" << std::endl;
  } else  {
    // Trick to have static_assert on "bad" types only 
    //Use "good" type from above
    static_assert(std::is_floating_point_v<T>, "bad type"); 
  }
}


int main()
{
 foo(5);
 foo(5.4);
 //foo(""); // error
 C c;
 //foo(c); error
 
  return 0;
}

