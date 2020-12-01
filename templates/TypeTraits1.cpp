// This is the main project file for VC++ application project 
// generated using an Application Wizard.

#include "stdafx.h"
#using <mscorlib.dll>

 
#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <functional>
#include <numeric>
using namespace std;
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>
using namespace std;

#include <exception>
using namespace std;
class NullType {};

template <typename T> class TypeTraits{
	 private:
	template <typename U> struct PointerTraits{
		enum{result = false};
		typedef NullType PointerType;
	};
	template <typename U> struct PointerTraits<U*>{
		enum{result = true};
		typedef U PointerType;
	};
public:
	enum{IsPointer = PointerTraits<T>::result};
	typedef typename PointerTraits<T>::PointerType PointerType;
	

};

template <class T> bool isP(T*){return true;}
template <class T> bool isP(T){return false;}

int main()
{
   int y =1;
   cout <<isP(y)<<endl;
   cout <<isP(&y)<<endl;
   cout << TypeTraits<vector<int>::iterator> ::IsPointer<<endl;
return 0;
}