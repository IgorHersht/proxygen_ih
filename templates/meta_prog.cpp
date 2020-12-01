// Copyright (C) 2001-2003
// William E. Kempf
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/thread/thread.hpp>
#include <boost/thread/once.hpp>
#include <iostream>
using namespace std;



template <int n> struct F{
  
    enum {Result = n * F<n -1>::Result};   
};

template <> struct F<1>{
  
    enum {Result = 1};   
};


int main(int argc, char* argv[])
{
cout<< F<3>::Result<<endl;
}
