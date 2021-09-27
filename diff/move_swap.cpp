#include <utility>
#include <iostream>
#include <memory>

template<typename T> void swap(T& a, T& b)
{
T tmp(std::move(a));
a = std::move(b);
b = std::move(tmp);
}

struct S {

	std::unique_ptr<int> d{new int(10)};
};


int main(int argc, char* argv[])
{
  std::cout << "Exception: " <<std::endl;
  S s1; S s2;
  *(s1.d) = 5;
  int i1 = *(s1.d);
   int i2 = *(s2.d);
  swap(s1,s2);
  i1 = *(s1.d);
  i2 = *(s2.d);
  int j = i1 + i2;


  return 0;
}
