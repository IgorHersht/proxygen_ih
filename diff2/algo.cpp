#include <set>
#include <vector>
#include <iostream>
#include <string>

struct Pr {
	int tDs = 0; // 
	int in = 0;
	int dp = 0;
	int diff = 0;
	bool ll = false;
};

Pr get(int tDs, int dpMin) {
	Pr pr;
	pr.tDs = tDs;
	pr.in = (tDs) / dpMin ;
	pr.dp = tDs / (pr.in) ;
	pr.diff = tDs - pr.in * pr.dp ;
	pr.ll = ((pr.in - pr.diff) > 0);

	
	return pr;


}

int main()
{
	std::vector< Pr > v;
	for (int i = 25; i < 365; ++i) {
		v.emplace_back(get(i, 24));
	}

	for (auto v : v) {
		std::cout << "tDs="<<v.tDs << " " << "in=" << v.in << " dp=" << v.dp << " diff=" << v.diff << " ll=" << v.ll <<std::endl;
	}

	for (auto v : v) {
		if(!v.ll) std::cout << "XXXXXXXXXXXXXXXXX"<< std::endl;
	}
	
	std::cout << "OK" << "\n";
}
