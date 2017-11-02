#include <future>
#include <thread>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <chrono>
#include <condition_variable>
#include <thread>
using namespace std;
/*
 * Waiting in several threads for data
 */

int queryNumber ()
{
	 std::this_thread::sleep_for(std::chrono::seconds(1));

    return 10;
}

void wait (char c, shared_future<int> f){
        // wait for number of characters to print
        int num = f.get();  // get result of queryNumber()
        std::this_thread::sleep_for(std::chrono::seconds(10));
}

void wait_for_shared_future(){// can start several threads in parallel
		// start one thread to query a number
		shared_future<int> f = async(launch::async,queryNumber);

		// start three threads each processing this number in a loop
		auto f1 = async(launch::async,wait,'.',f);
		auto f2 = async(launch::async,wait,'+',f);
		auto f3 = async(launch::async,wait,'*',f);

		// wait for all loops to be finished
		f1.get();
		f2.get();
		f3.get();

}

//////////////////////////////////////////



std::condition_variable cv;
std::mutex cv_m;
int i = 0;

void waits()
{
    std::unique_lock<std::mutex> lk(cv_m);
    cv.wait(lk, []{return i == 1;});
    std::this_thread::sleep_for(std::chrono::seconds(10));
}

void signals()
{

    std::this_thread::sleep_for(std::chrono::seconds(1));

    {
        std::lock_guard<std::mutex> lk(cv_m);
        i = 1;
    }
    cv.notify_one();
}

void wait_for_condition(){
	 std::thread t1(waits), t2(waits), t3(waits), t4(signals);
	    t1.join();
	    t2.join();
	    t3.join();
	    t4.join();

}



int main()
{
	/////
	auto start = std::chrono::system_clock::now();
	wait_for_shared_future();// can start several threads in parallel
	auto end = std::chrono::system_clock::now();
	cout << "wait_for_shared_future "<< chrono::duration_cast<chrono::seconds>(end - start).count() <<std::endl;
	/////////////
	start = std::chrono::system_clock::now();
	wait_for_condition();// cannot start several threads in parallel
	end = std::chrono::system_clock::now();
	cout << "wait_for_condition "<< chrono::duration_cast<chrono::seconds>(end - start).count() <<std::endl;
}
/*
 wait_for_shared_future 11
wait_for_condition 31
 *
 */

