namespace A {
    template<typename T> concept C = requires {
        typename T::type;
    };
    template<typename T> struct S {
        void f() requires C<T>;
        void g() requires C<T>;
    };
    template<typename T> void S<T>::f() requires C<T> { } // OK
    //template<typename T> void S<T>::g()  { } error
}

namespace B {
    template<typename T> concept C = true;
    template<typename T> concept D = true;
    template<C T> struct S {
        void f();
        void g();
        void h();
        template<D U> struct Inner;
    };
    template<C A> void S<A>::f() { } // OK, template-heads match
    //template<typename T> void S<T>::g() { } // error: no matching declaration for S<T>
    template<typename T> requires C<T> void S<T>::h() {
        int i = 1;
    } // ill-formed, no diagnostic required: template-heads are functionally equivalent but not equivalent;
    template<C X> template<D Y> struct S<X>::Inner { }; // OK

}



int main() {
    B::S<int> s1;
    s1.h();

}



