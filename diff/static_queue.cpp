#include <deque>
#include <list>
#include <set>
#include <map>
#include <boost/fusion/algorithm/transformation/transform.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/container/map.hpp>
 
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
 
#include <algorithm>
 
#include <iostream>
#include <ostream>
#include <vector>
 
using namespace boost;
using namespace std;
 
// ___________________________________________________________________ //
namespace implementation
{
 
template<typename Types>
struct static_poly_seq
{
    typename fusion::result_of::as_map
    <
        typename mpl::transform
        <
            Types,
            fusion::pair< mpl::_1, std::vector<mpl::_1> >
        >::type 
    >::type vectors;
};
 
template<typename Types,typename Type> inline
void push_back_2D(static_poly_seq<Types> &seq,const Type &value)
{
    fusion::at_key< Type >(seq.vectors).push_back(value);
}
 
template<typename PolymorhicUnaryFunctor>
class for_each_of_range
{
    mutable PolymorhicUnaryFunctor f;
public:
    for_each_of_range(PolymorhicUnaryFunctor f_) : f(f_) {}
    template<typename Range> void operator()(Range &r) const
    {
        for_each(r.second.begin(),r.second.end(),f); // There is no Boost.Range on Ideone
    }
};
 
template<typename Types,typename PolymorhicUnaryFunctor> inline
void for_each_2D(static_poly_seq<Types> &seq,PolymorhicUnaryFunctor f)
{
    fusion::for_each
    (
        seq.vectors,
        for_each_of_range<PolymorhicUnaryFunctor>(f)
    );
}
 
}
// ___________________________________________________________________ //
 
struct Printer
{
    void operator()(int i) const
    {
        cout << "integer = " << i << endl;
    }
    void operator()(double d) const
    {
        cout << "double = " << d << endl;
    }
};
 
int main (int argc, char ** argv)
{
    using namespace implementation;
    static_poly_seq< mpl::vector<int,double> > seq;
    for(int i=0;i!=3;++i)
    {
        push_back_2D(seq,i);
		if(argc == 1){
			push_back_2D(seq,i*10.0);
		}else{
			push_back_2D(seq,i);
		}
    }
    for_each_2D(seq,Printer());
}
