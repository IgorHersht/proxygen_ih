//Template argument deduction for class templates
template <typename T = float>
struct MyContainer {
  T val;
  MyContainer() : val() {}
  MyContainer(T val) : val(val) {}
  // ...
};
MyContainer c1{ 1 }; // OK MyContainer<int>
MyContainer c11( 1 ); 
MyContainer c12= 1 ; 
MyContainer c2; // OK MyContainer<float>
////
// Folding expressions
/*
A fold expression performs a fold of a template parameter pack over a binary operator.

    An expression of the form (... op e) or (e op ...), where op is a fold-operator and e is an unexpanded parameter pack, are called unary folds.
    An expression of the form (e1 op1 ... op2 e2), where op1 and op2 are fold-operators, is called a binary fold. Either e1 or e2 are unexpanded parameter packs, but not both.

*/

template<typename... Args>
bool logicalAnd(Args... args) {
    // Binary folding.
    return (true && ... && args);
}
bool b = true;
bool& b2 = b;


template<typename... Args>
auto sum(Args... args) {
    // Unary folding.
    return (... + args);
}

/*
New rules for auto deduction from braced-init-list

Changes to auto deduction when used with the uniform initialization syntax. Previously, auto x{ 3 }; 
deduces a std::initializer_list<int>, which now deduces to int.

*/

 #include <initializer_list>
  #include <iostream>
 struct S{
 S(int){
 std::cout <<"S::S(int)";
 }
 S(std::initializer_list<int>){
  std::cout <<"S::S(std::initializer_list<int>)";
 }
}; 
//auto x1{ 1, 2, 3 }; // error: not a single element
auto x2 = { 1, 2, 3 }; // decltype(x2) is std::initializer_list<int>
auto x3{ 3 }; // decltype(x3) is int
auto x4{ 3.0 }; // decltype(x4) is doubl
S s1{5};// but S::S(std::initializer_list<int>)
S s2(5);// but S::S(int)

int main(){
logicalAnd(b, b2, true); // == true
sum(1.0, 2.0f, 3); // == 6.0

}
