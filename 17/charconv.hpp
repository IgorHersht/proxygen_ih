
#include <iostream>
#include <charconv>
#include <cassert>

void d2str2d(double value1)
{
  std::cout << "in:  " << value1 << '\n';

  // convert to character sequence:
  char str[1000];
  std::to_chars_result res1 = std::to_chars(str, str+999, 
                                            value1);
  *res1.ptr = '\0';  // add trailing null character

  std::cout << "str: " << str << '\n';
  assert(res1.ec == std::errc{});

  // read back from character sequence:
  double value2;
  std::from_chars_result res2 = std::from_chars(str, str+999, 
                                                value2);

  std::cout << "out: " << value2 << '\n';
  assert(res2.ec == std::errc{});

  assert(value1 == value2);  // should never fail
}

