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
template <int v>
struct Int2Type{
	enum {value = v};
};

template <typename T>f(const T& obj){cout<<"T "<<obj.value<<endl;}
void f(const Int2Type<1>& obj){cout<<"1"<<obj.value<<endl;}
void f(const Int2Type<2>& obj){cout<<"2"<<obj.value<<endl;}


int main()
{
	f(Int2Type<1>());
	f(Int2Type<2>());
	f(Int2Type<3>());
 
	return 0;
} 



