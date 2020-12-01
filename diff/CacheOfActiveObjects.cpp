#include <iostream>
#include <mutex>
#include<string>
#include <memory>
#include<map>

/*
 * You have bunch of “Big Items” which used from several threads.
You want have only one instance of an item at the same time.
You want to release memory when you are done.
 */

struct BigItemI{
    virtual void use() =0;
};

enum ItemId{Item1, Item2, Item3 };
template <ItemId id> struct BigItem;


template <> struct BigItem<Item1> : public BigItemI{
    virtual void use() override {
    }
    /// a lot of data
};

template <> struct BigItem<Item2> : public BigItemI{
    virtual void use() override {
    }
    /// a lot of data
};

template <> struct BigItem<Item3> : public BigItemI{
    virtual void use() override {
    }
    /// a lot of data
};

struct BigItems{
    template <ItemId id> std::shared_ptr<BigItemI> get(){
        std::lock_guard lc(_m);
        auto sp = _items[id].lock();
        if(!sp){
            sp = std::make_shared<BigItem>();
            _items[id] = sp;
        }
        retur sp;
    }
private:
 std::mutex _m;
 std::map<int, std::weak_ptr<BigItemI> > _items;
};

int main()
{


    return 0;
}


