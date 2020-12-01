#include <map>
#include <iostream>
#include <string>
/* https://en.cppreference.com/w/cpp/container/map/merge
 * Attempts to extract ("splice") each element in source and insert it into *this using the comparison object of *this.
 * If there is an element in *this with key equivalent to the key of an element from source, then that element is not extracted from source.
 * No elements are copied or moved, only the internal pointers of the container nodes are repointed.
 * All pointers and references to the transferred elements remain valid, but now refer into *this, not into source.

The behavior is undefined if get_allocator() != source.get_allocator().
 */
nt main(int argc, char* argv[])
{
    //////extract
    std::map<int, std::string> m{{1, "mango"}, {2, "papaya"}, {3, "guava"}};
    auto nh = m.extract(2);
    nh.key() = 4;
    m.insert(move(nh));
// m == {{1, "mango"}, {3, "guava"}, {4, "papaya"}}
    //merge
    std::map<int, std::string> ma {{1, "apple"}, {5, "pear"}, {10, "banana"}};
    std::map<int, std::string> mb {{2, "zorro"}, {4, "batman"}, {5, "X"}, {8, "alpaca"}};
    std::map<int, std::string> u;
    u.merge(ma);
    std::cout << "ma.size(): " << ma.size() << '\n';
    u.merge(mb);
    std::cout << "mb.size(): " << mb.size() << '\n';
    std::cout << "mb.at(5): " << mb.at(5) << '\n';
    for(auto const &kv: u)
        std::cout << kv.first << ", " << kv.second << '\n';

    ///

    std::map<int, std::string> mc {{1, "apple"}, {5, "pear"} };
    std::string a2= "apple2"; std::string p2= "pear2";
    mc.try_emplace(3, std::move(a2)); // move a2 and insert into map
    mc.try_emplace(1, std::move(p2)); // no move a2 and no insert into map
    int i =1;

}
/*
 ma.size(): 0
mb.size(): 1
mb.at(5): X
1, apple
2, zorro
4, batman
5, pear
8, alpaca
10, banana
 *
