//Question 1.
#include<iostream>
#include<boost/tokenizer.hpp>
#include<string>

void printWords(const std::string& originalText){
    boost::tokenizer<> tok(originalText);
    for(const auto& w: tok ){
        std::cout << w << std::endl;
    }
}
//test
int main(){
    printWords("Hello World");
    return 0;
}
//Question 2
/*
 * Don’t understand question 2 at all
1.	UDP is not stream
2.	What is redundant UDP stream?
3.	Order by seqnum or receiving order?
4.	Meaning of const char stream not clear.

 */

//Question 3
#include <vector>
#include <bitset>
#include <set>

struct Quete{
    int Source = 0;
    double Price = 0.0;
    double Quantity = 0.0;
};
/*(
 * Assume Source cannot be a big number.
 */
template<size_t MaxSrc = 64> struct Filter{
 explicit Filter(const std::set<size_t>& goods  ){
     for(size_t v: goods ){
         if(v >= MaxSrc ){
             // error msg
         }else{
             _goods.set(v);
         }
     }
 }
 // assume output can life longer than input => need copy
 // assume Quete is a small => vector of Quete ( not pointers)
 std::vector<Quete> apply(const std::vector<Quete> in) const{
     std::vector<Quete> out;
     for( const Quete& q: in){
         const auto src= q.Source;
         if(src >= MaxSrc ){
             // error msg
         }else if(_goods.test(src)){
            out.push_back(q);
         }
     }
     return out;
 }
private:
    std::bitset<MaxSrc> _goods;
};

//
//test
int main()
{
    Filter filter{{1,4}};
    std::vector<Quete> in{{1,1.2, 1.3},{1,2.2, 2.3},{2,2.2, 2.3},{4,4.2, 4.3} };
    std::vector<Quete> out = filter.apply(in);
   return 0;
}

