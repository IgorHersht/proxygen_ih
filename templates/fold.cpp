
https://www.fluentcpp.com/2021/03/19/what-c-fold-expressions-can-bring-to-your-code/
template<typename... Values>
auto average(Values const&... values)
{
    constexpr auto numberOfValues = double{sizeof...(values)};
    static_assert(numberOfValues > 0);
    return (... + (values / numberOfValues));
}

template<typename T, typename... Ts>
void push_back(std::vector<T>& v, Ts&&... values)
{
    (v.push_back(std::forward<Ts>(values)), ...);
}

template<typename Function, typename... Values>
auto for_each_arg(Function function, Values&&... values)
{
    return (function(std::forward<Values>(values)),...);
}