#include <iostream>
#include <string_view>
#include <utility>


int main()
{
    std::string_view data = "abcxxxxxxxyyyy";

    auto trim_pos = data.find("yyyy");
    if(trim_pos != data.npos)
        data.remove_suffix(data.size() - trim_pos);
    std::cout << data << std::endl;


    return 0;
}

