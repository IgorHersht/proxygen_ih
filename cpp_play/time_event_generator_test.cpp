
#include "time_event_generator.h"
#include<iostream>
#include<chrono>
static std::mutex io_mutex;
void th1(){
	 std::unique_lock<std::mutex> lc(io_mutex);
	std::cout << "th1"<<std::endl;
}
void th2(){
	std::unique_lock<std::mutex> lc(io_mutex);
	std::cout << "th2"<<std::endl;
}

void dh1(){
	 std::unique_lock<std::mutex> lc(io_mutex);
	std::cout << "dh1"<<std::endl;
}
void dh2(){
	std::unique_lock<std::mutex> lc(io_mutex);
	std::cout << "dh2"<<std::endl;
}


int main(){

	auto context= std::make_shared<boost::asio::io_context>(1);

	cpp_tools::TimeEventGenerator at_generator(context);
	auto time = boost::posix_time::second_clock::universal_time();
	at_generator.add_timer(time + boost::posix_time::seconds(5), th1 );
	at_generator.add_timer(time + boost::posix_time::seconds(7), th2 );

	cpp_tools::DurationTimeEventGenerator every_generator(context);
	every_generator.add_timer(boost::posix_time::seconds(1), dh1 );
	every_generator.add_timer( boost::posix_time::seconds(2), dh2 );

	std::thread th([context]{context->run(); });

	std::this_thread::sleep_for(std::chrono::seconds(31));
	at_generator.stop();
	every_generator.stop();
	context->stop();

	th.join();


	return 0;
}
