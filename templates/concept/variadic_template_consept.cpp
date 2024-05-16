template<typename T> concept C1 = true;
template<typename... Ts> concept C2 = true;
template<C1 T> struct s1; // associates C1<T>
template<C1... T> struct s2; // associates (C1<T> && ...)
template<C2... T> struct s3; // associates (C2<T> && ...)

int main() {

}
