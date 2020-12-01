#include <thread>
#include <iostream>
#include <memory>
#include<string>
#include <tuple>
#include<typeinfo>


int ar[] = {2,5,354,7,0,34};

int main(int argc, char** argv){
	int j = 5;
	for(auto v: ar){
		std::cout <<v <<":";
	}
	std::cout  <<std::endl;
	for(auto it = std::cbegin(ar), end = std::cend(ar); it != end; ++it){
		std::cout <<*it <<":";
	}
	std::cout  <<std::endl;
 return 0;
}

/*
2:5:354:7:0:34:
2:5:354:7:0:34:
 */


