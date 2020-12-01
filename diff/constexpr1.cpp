
#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <functional>
#include <numeric>
#include <vector>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>



 constexpr int factorial(int n)
{
    return n <= 1 ? 1 : (n * factorial(n-1));
}
 
 class Circle
{
    public:
    constexpr Circle (double radius) :  _radius( radius ) {}
    constexpr double getArea () 
    {
        return _radius * _radius * 3.1415926;
    }
    private:
        int _radius;
};



int main(int, char*[])
{
    int j = factorial(5);
    constexpr Circle c1(3.5);
    constexpr double er = c1.getArea();
    return 0;
}
 
