

#include <iostream>
#include <type_traits>
#include<memory>
#include<string>
#include<vector>

struct MovableType {

	MovableType() {
		int i = 1;
	}

	MovableType(MovableType &&) {
		int i = 1;
	}
	MovableType& operator =(MovableType &&) {
		int i = 1;
	}

};


struct CopyType {

	CopyType() {
		int i = 1;
	}

	CopyType(const CopyType &) {
		int i = 1;
	}
	CopyType& operator =(const CopyType &) {
		int i = 1;
	}
	int m_i{ 0 };
};


struct CopyMType {

	CopyMType() {
		int i = 1;
	}

	CopyMType(const CopyMType &) {
		int i = 1;
	}
	CopyMType& operator =(const CopyMType &) {
		int i = 1;
	}


	CopyMType(CopyMType &&) {
		int i = 1;
	}
	CopyMType& operator =(CopyMType &&) {
		int i = 1;
	}

};


struct Widget {

	MovableType m_m;
	CopyType    m_c;
	CopyMType   m_cm;
};


int main(int argc, char* argv[]) {

	
	Widget w1;
	Widget w2(std::move(w1)); // Will move MovableType and CopyMType. Will copy CopyType;
	//Widget w3(w2); // error cannot move CopyType

	return 0;
}


