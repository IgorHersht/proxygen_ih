 #include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

template <class Iterator>
  struct iterator_traitss {
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type        value_type;
    typedef typename Iterator::difference_type   difference_type;
    typedef typename Iterator::pointer           pointer;
    typedef typename Iterator::reference         reference;
  };

  template <class T>
  struct iterator_traitss<T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T                          value_type;
    typedef ptrdiff_t                  difference_type;
    typedef T*                         pointer;
    typedef T&                         reference;
  };

  template <class CT, typename T = typename CT::value_type> struct ElementType{
	  typedef T Type;
  };


int main() 
{
	iterator_traitss<int*>::value_type tt = 1;
	ElementType< vector <int> >::Type t1 =6;
	return 0;
}

