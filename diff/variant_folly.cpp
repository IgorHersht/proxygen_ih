/*
#include <folly/Exception.h>

#include <folly/test/gtest-1.7.0/include/gtest/gtest.h>
#include <folly/portability/GTest.h>

#include <cstdio>
#include <memory>
*/
#include<queue>
#include<iostream>

#include <glog/logging.h>

#include <folly/DiscriminatedPtr.h>

using namespace folly;

struct Foo { };
	  struct Visitor {
	    void operator()(int* /* ptr */) { result = "int"; }
	    void operator()(const int* /* ptr */) { result = "const int"; }
	    void operator()(Foo* /* ptr */) { result = "Foo"; }
	    void operator()(const Foo* /* ptr */) { result = "const Foo"; }

	    std::string result;
	  };

	  typedef DiscriminatedPtr<int, Foo> Ptr;

	  Visitor v;
	  std::queue<Ptr> qq;

int main(){


	  int a = 0;
	  Ptr p_in;
	  p_in.set(&a);
	  qq.push(p_in);
	  //
	  Foo foo;
	  p_in.set(&foo);
	  qq.push(p_in);
	  //////////////////////

	  Ptr p_out = 	  qq.front();
	  qq.pop();
	  p_out.apply(v);
	  std::cout << v.result<< std::endl;

	  p_out = 	  qq.front();
	  qq.pop();
	  p_out.apply(v);
	  std::cout << v.result<< std::endl;


	return 0;
}
/*
int
Foo
 */

