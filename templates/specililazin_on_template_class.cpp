
template<typename T1, typename T2> class C {};

template<typename T> struct is_c :public std::false_type {};
template<typename T1, typename T2> struct is_c<C<T1,T2>> :public std::true_type {};// specilization
static_assert(is_c< C<int,char>>::value);
template <typename T> constexpr static bool is_c_v = is_c<T>::value;
template <typename T> concept same_as_c = is_c_v<T>;

int main() {
}

