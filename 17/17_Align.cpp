#include <iostream>
#include <new>

constexpr int al = 256;

class alignas(al) Vec3d {
};

int main() {
  int i = std::hardware_destructive_interference_size;//64
  int i2 = std::hardware_constructive_interference_size;//64
  auto Vec = Vec3d{};
  auto pVec = new Vec3d;

  if((size_t)&Vec % al == 0)
    std::cout << "aligned\n";
  else
    std::cout << "not aligned \n";

  if((size_t)(pVec) % al == 0)
    std::cout << "aligned\n";
  else
    std::cout << "not aligned \n";
  delete pVec;
}

