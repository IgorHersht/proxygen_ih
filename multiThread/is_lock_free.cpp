#include <iostream>
#include <utility>
#include <atomic>

struct A { int a[4]; };//lock free 16 bytes
struct B { long x, y; };///lock free 16 bytes
struct C { char c[16]; };///lock free 16 bytes
struct D { char d[17]; };/// not lock free 17 bytes
struct E { char e; long l; char e2;  };/// not lock free 24 bytes (because of pudding)

int main()
{
    std::atomic<A> a;
    std::atomic<B> b;
    std::atomic<C> c;
    std::atomic<D> d;
    std::atomic<E> e;
    int sz = sizeof(E);

    std::cout << std::boolalpha << "std::atomic<A> is lock free? "<< std::atomic_is_lock_free(&a) << '\n'
              << "std::atomic<B> is lock free? " << std::atomic_is_lock_free(&b) << '\n'
			  << "std::atomic<C> is lock free? " << std::atomic_is_lock_free(&c) << '\n'
    			<< "std::atomic<D> is lock free? " << std::atomic_is_lock_free(&d) << '\n'
				<< "std::atomic<E> is lock free? " << std::atomic_is_lock_free(&e) << '\n';
}

