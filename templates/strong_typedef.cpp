#include <map>
#include <assert.h>
#include<string>

template <typename T, typename U> struct strong_typedef{
    explicit constexpr strong_typedef( T value ):m_value(value){}
    auto operator<=> (const strong_typedef& ) const = default;// cannot be constexpr for std::string for my compiler ( no constexpr std::string yet)
    constexpr T value() const {
        return m_value;
    }
    constexpr const T& const_ref() const {
        return m_value;
    }
private:
    T m_value{};
};


using IntOne = strong_typedef<int, struct One>;
using IntTwo = strong_typedef<int, struct Two>;
using StringThree = strong_typedef<std::string, struct Three>;
using StringFour = strong_typedef<std::string, struct Four>;

void foo(IntOne intOne, IntTwo intTwo, StringThree stringThree, StringFour stringFour ){
    //intOne < intTwo ; // error
    //(intOne != intTwo ); // error
   // IntOne i1 = intTwo; // error
    //intOne = intTwo; // error
    //IntOne i1 = 5; // error
    //intOne =5; // error
   //stringThree != stringFour ;// error
    //intOne != intTwo ;
    std::string s3 = stringThree.value();
    const std::string& src =stringFour.const_ref();
    int i =1;
}



int main(){
    IntOne i1(1);
    std::string s3("3");
    foo(i1, IntTwo(2), StringThree(std::move(s3)), StringFour("4"));
 //   foo(IntTwo(2), i1,  StringThree(std::move(s3)), StringFour("4")); // error

    std::map<StringThree, int> m1;
    m1.try_emplace(StringThree("5"), 25);
    int i =1;

}

