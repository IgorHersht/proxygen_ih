// default.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;
#include <string.h>


  
template <class T> bool My_eq(T ar1, T ar2){
	return (ar1 == ar2)? true:false; 
}

template <> bool My_eq<int>(int ar1, int ar2){
	return (ar1 == ar2)? true:false; 
}
 bool My_eq(int ar1, int ar2){
	return (ar1 == ar2)? true:false; 
}
int _tmain(int argc, _TCHAR* argv[])
{	
	
	bool b1 =My_eq(2.2,3.3);
	bool b2 =My_eq(2,3);
	return 0;
}

