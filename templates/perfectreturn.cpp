
#include <functional>   // for std::forward()
#include <type_traits>  // for std::is_same<> and std::invoke_result<>

template<typename Callable, typename... Args>
decltype(auto) call(Callable op, Args&&... args)
{
  if constexpr(std::is_void_v<std::invoke_result_t<Callable, Args...>>) {
    // return type is void:
    op(std::forward<Args>(args)...);
    //...  // do something before we return
    return;
  }
  else {
    // return type is not void:
    decltype(auto) ret{op(std::forward<Args>(args)...)};
    //...  // do something (with ret) before we return
    return ret;
  }
}

