#include <iostream>
#include <string_view>
#include <utility>

constexpr std::array ar{
        std::string_view("abc"),
        std::string_view("efg")
};


int main()
{
    std::string_view data = "abcxxxxxxxyyyy";

    auto trim_pos = data.find("yyyy");
    if(trim_pos != data.npos)
        data.remove_suffix(data.size() - trim_pos);
    std::cout << data << std::endl;

	for(const auto& s: ar){
        std::cout << s << std::endl;
    }


    return 0;
}

