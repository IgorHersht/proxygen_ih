#include <iostream>
#include <string>
#include <map>

int main()
{
    std::map<int, std::string> m{{1, "mango"},
                                 {2, "papaya"},
                                 {3, "guava"}};
    int sz = std::size(m);// sz = 3
    auto nh = m.extract(2);  // move the node if the key exists, nh has type decltype(m)::node_type
    if(nh){
        nh.key() = 4;
        m.insert(std::move(nh));
        sz = std::size(m);// sz = 2
    }else{
        sz = std::size(m);// sz = 3
    }
    
    for (const auto& [key, value] : m) {
        std::cout << key << ": " << value << '\n';
    }
}

