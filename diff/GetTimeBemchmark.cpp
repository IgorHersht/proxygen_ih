

#include <iostream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>



constexpr int iter = 100000;


std::vector<ulong> data(iter);

int main(int argc, char** argv){


	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	for(int i = 0; i < iter; ++i){
		data[i] = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count();
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
 * Iter took 146.68 ns
 */


