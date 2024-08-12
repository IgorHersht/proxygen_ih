#include<functional>
// Lambda template specialisation - not good
void foo(std::function<void()> f) {
   f();
}
auto l1 = [] <typename T>(){ };

template <typename T> struct C {
   void operator()() {
   }
};

template <typename T > auto l2 = []() {
   int i = 1;
};

int main() {
   // no lambda param
   l1.operator()<int>();//!!! the only way

   foo(C<int>());// struct  works
   foo(l2<int>);// variable template works
}
