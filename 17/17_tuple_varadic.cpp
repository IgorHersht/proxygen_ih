#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>
#include <memory>

template <class... Args> struct C {
    C(Args&& ... args) :
        m_args(std::forward<Args>(args) ...) {
    }
    template <typename F> void invoce(F&& f) {
        std::apply(std::forward<F>(f), std::forward<decltype(m_args)>(m_args));
    }
    void useAll() {
        invoce([this](int i, double d) {  _useAll(i, d); });
    }
    template <size_t index, typename T> void set(const T& t) {
        static_assert(std::is_same_v<T, std::decay_t<decltype(std::get<index>(m_args))> >, "wrong type");
        std::get<index>(m_args) = t;
    }
    template<size_t index >decltype(auto) get() const {
        return std::get<index>(m_args);
    }
private:
    void _useAll(int i, double d) {
        std::cout << i << " " << d << std::endl;
    }

private:
    std::tuple<Args ...> m_args;
};

/// test
void global(int& ri, int i, double d) {
    ri = i + d;
}
struct A {
    void member(int i, double d) {
        std::cout << i << " " << d << std::endl;
    }
};


int main()
{
    int i = 0;
    C c1(std::ref(i), 5, 4.8);
    c1.invoce(global);

    C c(5, 4.8);
    c.invoce([a = std::make_unique<A>()](int i, double d) { a->member(i, d); });
    c.useAll();
    c.set<0, int>(7);
    //c.set<3, int>(7); // compile error wrong type
    auto v = c.get<0>();
    int j = v;
    return 0;
}