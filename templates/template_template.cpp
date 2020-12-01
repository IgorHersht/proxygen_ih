#include <string>
#include <vector>
#include <iostream>

template <class T1> class MyVector{};

template <class T1, template<typename T> class V1, template<typename T, typename = std::allocator<T> > class Container>
struct B{
	Container<T1> m_contaner;
	V1<T1> v;
};

B<int, MyVector, std::vector > b1;
int main() {
 

}
