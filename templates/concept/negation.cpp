//
// Created by igersht on 5/15/2024.
//

//A logical negation expression (7.6.2.2) is an atomic constraint; the negation operator is not treated as a
//logical operation on constraints.
// test
template <class T> concept sad = false;
template <class T> int f1(T) requires (!sad<T>);
template <class T> int f1(T) requires (!sad<T>) && true;
//int i1 = f1(42); // ambiguous, !sad<T> atomic constraint expressions (13.5.2.3)
// are not formed from the same expression
template <class T> concept not_sad = !sad<T>;
template <class T> int f2(T) requires not_sad<T>;
template <class T> int f2(T) requires not_sad<T> && true;
int i2 = f2(42); // OK, !sad<T> atomic constraint expressions both come from not_sad
int main() {

}