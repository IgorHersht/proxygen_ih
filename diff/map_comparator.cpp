#include <string>
#include <map>
#include <iostream>
#include <memory>

struct Catalog {
    struct Key {
        uint8_t _topic{};
        uint8_t _lap{};
        uint16_t _subTopic{};
        uint64_t _aux{};
    };
    struct Comparator {
        bool operator()(Key l, Key r) const {
            if (l._topic < r._topic) {
                return true;
            }
            else if (l._topic > r._topic) {
                return false;
            }

            if (l._subTopic < r._subTopic) {
                return true;
            }
            else if (l._subTopic > r._subTopic) {
                return false;
            }

            if (l._lap < r._lap) {
                return true;
            }
            else if (l._lap > r._lap) {
                return false;
            }

            return (l._aux < r._aux);
        }

    };

    using Map = std::map<Key, bool, Comparator >;
    void emplace(Key key, bool value) {
        auto [iter, status] = _map.emplace(key, value);
        if (!status) {
            iter->second = value;
        }
    }

    bool isSet(Key key) const {
        auto iter = _map.find(key);
        if (iter == _map.end()) {
            return false;
        }
        return iter->second;
    }
private:
    Map _map;
};


int main() {

    Catalog::Key k1{ 2, 0, 3,5 };
    Catalog::Key k2{ 2, 0, 3,4 };
    Catalog ct;
    ct.emplace(k1, true);

    bool st1 = ct.isSet(k1);
    bool st2 = ct.isSet(k2);
    int i = 1;

    return 0;
}
