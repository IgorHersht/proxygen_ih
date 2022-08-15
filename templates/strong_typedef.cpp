#include <map>
#include <assert.h>
#include<string>
#include<string_view>
#include<array>
#include<type_traits>

template<typename T> concept ShouldBeCopied = std::is_trivially_copy_assignable_v<T>;
template<typename T> concept ShouldBeMoved = !ShouldBeCopied<T>;

template < typename T, typename U>
struct strong_typedef{

    explicit constexpr strong_typedef( T value ) requires (ShouldBeCopied<T>)
            :m_value(value){
        int i =1;
    }
    explicit constexpr strong_typedef( T&& value ) requires (ShouldBeMoved<T>)
            :m_value(std::move(value)){
        int i =1;

    }

    auto operator<=> (const strong_typedef& ) const = default;
    constexpr T value() const requires (ShouldBeCopied<T>) {
        return m_value;
    }
    constexpr const T& const_ref() const requires (ShouldBeMoved<T>) {
        return m_value;
    }
private:
    T m_value{};
};



std::array ar1{1,2};
std::array ar2{1,3};
using ArOne = strong_typedef<decltype(ar1), struct One>;
using ArTwo = strong_typedef<decltype(ar2), struct Two>;

using IntOne = strong_typedef<int, struct One>;
using IntTwo = strong_typedef<int, struct Two>;
using StringThree = strong_typedef<std::string, struct Three>;
using StringFour = strong_typedef<std::string, struct Four>;

void foo(IntOne intOne, IntTwo intTwo, StringThree stringThree, StringFour stringFour, ArOne arOne, ArTwo arTwo  ){
    //intOne < intTwo ; // error
    //(intOne != intTwo ); // error
    // IntOne i1 = intTwo; // error
    //intOne = intTwo; // error
    //IntOne i1 = 5; // error
    //intOne =5; // error
    //stringThree != stringFour ;// error
    //intOne != intTwo ;
    std::string s3 = stringThree.const_ref();
    const std::string& src =stringFour.const_ref();
    int i =1;
}




int main(){
    IntOne i1(1);
    std::string s3("3");
    ArOne arOne(ar1);
    foo(i1, IntTwo(2), StringThree(std::move(s3)), StringFour("4"), arOne, ArTwo(ar2) );
    //   foo(IntTwo(2), i1,  StringThree(std::move(s3)), StringFour("4"), arOne, ArTwo(ar2)); // error
    //foo(i1, IntTwo(2), StringThree(std::move(s3)), StringFour("4"), ArTwo(ar2), arOne  );

    std::map<StringThree, int> m1;
    m1.try_emplace(StringThree("5"), 25);
    constexpr auto r = IntOne(5) <=> IntOne(6);


    int i =1;

}

