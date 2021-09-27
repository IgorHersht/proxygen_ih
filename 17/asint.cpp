
#include <optional>
#include <string_view>
#include <charconv>     // for from_chars()
#include <iostream>

// convert string to int if possible:
std::optional<int> asInt(std::string_view sv)
{
  int val;
  // read character sequence into the int:
  auto [ptr, ec] = std::from_chars(sv.data(), sv.data()+sv.size(),
                                   val);
  // if we have an error code, return no value:
  if (ec != std::errc{}) {
    return std::nullopt;
  }
  return val;
}

int main()
{  
  for (auto s : {"42", "  077", "hello", "0x33"} ) {
    // try to convert s to int and print the result if possible:
    std::optional<int> oi = asInt(s);
    if (oi) {
      std::cout << "convert '" << s << "' to int: " << *oi << "\n";
    }
    else {
      std::cout << "can't convert '" << s << "' to int\n";
    }
  }
}

