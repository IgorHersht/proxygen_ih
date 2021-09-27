

#include <iostream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>



int roll_die( boost::random::mt19937 &gen, int begin, int end) {
    boost::random::uniform_int_distribution<> dist(begin, end);
    return dist(gen);
}

constexpr int iter = 100000;


std::vector<int> data(iter);

int main(int argc, char** argv){
	//once per process
	boost::random::mt19937 gen0;
	int random_seed1 = roll_die(gen0, 0,100000); // should be generated for every thread

	// per thread
	boost::random::mt19937 gen(random_seed1);//random_seed1 to have different starting conditions in diff threads

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	for(int i = 0; i < iter; ++i){
		data[i] = roll_die(gen,0, 20);
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	 for(int i = 0; i < iter; ++i){
			 std::cout << data[i]<< std::endl;
		 	}
	int total =  std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	 std::cout << "Loop took " <<total << " ms"<< std::endl;
	 std::cout << "Iter took " << ((double)total  / double(iter)) * 1000<< " ns"<< std::endl;


 return 0;
}
/*
 * Iter took 14.63 ns
 */



