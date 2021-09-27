#include<utility>
template< typename T>  struct Singlecton{
    static T& instance(){
        static T ins;
        return ins;
    }
};

// test
class C{
    friend class Singlecton<C>;
    C(){
        ++_i;
    }
    inline static int _i = 0;
};
void test(){
    C& c1 = Singlecton<C>::instance();
    C& c2 = Singlecton<C>::instance();
    C& c3 = Singlecton<C>::instance();
}

int main()
{
    test();
    return 0;
}

