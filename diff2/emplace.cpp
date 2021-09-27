

#include <vector>
#include <map>
#include <unordered_map>
#include <memory>
#include <memory>
#include <iostream>

struct C {

	C(int i):_i(i) {
		std::cout << "C()" << std::endl;
		throw std::string();
	}

	C(const C&) {
		std::cout << "C(const C&)" << std::endl;
	}

	C& operator =(const C&) {
		std::cout << "C & operator =(const C&)" << std::endl;
	}

	~C() {
		std::cout << "~C()" << std::endl;
	}
	friend bool operator <(const C& l, const C& r) {
		return (l._i < r._i);
	}
	int _i;

};



int main()
{
	std::vector<C> v;
	int i = 0;
	{
		//C c(1);
		//v.push_back(c);
		/*
		C()
		C(const C&) // 
		~C()
		*/
		
	}
	i = 1;
	{
		try {
			v.emplace_back(2);
		}
		catch (...) {
		}
		//;
		/*
		C()
		*/
	}
	i = v.size();  // 0

	std::map<C, int> m;
	{
		//m.insert(std::map < C, int>::value_type(C(1), 2));
		/*
		C()
		C(const C&)
		C(const C&)
		~C()
		~C()

		*/
	}
	i = 3;
	{
		/*
		try {
			m.emplace(C(1), 2);
		}
		catch (...) {
		}
		*/
		/*
		C()
		C(const C&)
		~C()

		*/
	}
	i = m.size(); // 0
	int j = 2;
	
}

