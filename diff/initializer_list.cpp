#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <algorithm>
#include <memory>
#include <initializer_list>
#include <array>

template<typename T, typename ... Args > std::unique_ptr<T> make_unique(Args&& ... args ){
	return new T(std::forward<Args> (args)...);
}

template <typename T> struct Array{

	 Array(std::initializer_list<T> l):m_size(l.size()){
		 std::unique_ptr<T[]>  data = std::make_unique< T[]>(m_size);
		 //T &datar = (*data);
		 size_t i = 0;
		 for( auto &e: l){
			 data[i]  = e;
			 ++i;

		 }
		 m_data = std::move(data);
	}


	size_t 					m_size;
	std::unique_ptr<T[]> 	m_data;
};



int main()
{
auto d1 = std::make_unique<int[]> (100);
std::unique_ptr<int[]> d2(new int[20]);

std::vector<int> v1{1,2,3};
std::vector<int> v2; v2.shrink_to_fit();
v2 = std::move(v1);
int s1 = v1.size();
v1 = std::move(v2);
Array<int> ar {5,6};
int e0 = ar.m_data[0];
int e1 = ar.m_data[1];

int ss = sizeof(std::string);

  return 0;
}

