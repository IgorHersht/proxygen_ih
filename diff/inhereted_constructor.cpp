#include <iostream>
#include <exception>
#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<list>
#include<vector>
#include<queue>
#include<algorithm>
#include<iterator>
#include<string>

struct B{
	B(int i):m_i(i){

	}
	B(){

		}
	int m_i = 1;
};

struct D : public B{
	using B::B;
};



int main(int argc, char* argv[]){

	D d0;
D d1(25);





 return 0;
}

