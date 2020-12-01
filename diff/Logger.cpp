#include <string>
#include <thread>
#include <iostream>
#include <fstream>
#include <boost/thread/sync_bounded_queue.hpp>

/*
 *Since no references for important parts of "real production" thing like time stamp of message, priority, location in source ( line number, function ) I assume that
 *this is a test with several assumptions;
 *1. input for log is const char*, const std::string &, std::string
 *2. If too many messages is comming than loosing message than better that blocking ( or slow down)  "main" threads.
 *3. Only boost is available
 *4. init exceptions ( like wrong log file name) should kill application ( bad configuration)
 *4.
 */



// assuming from config
std::string inline getLogFileName(){
	return "/home/ihersht/tmp/logoutput.log";
}

size_t inline getMaxPendingMsgs(){
	return 1000;
}
struct Logger{
	// more sofisticated ( in real life needed)
	static Logger& Instance(){
		static Logger logger(getLogFileName(), getMaxPendingMsgs()); // assume C++11 => thread safe
		return logger;
	}

	~Logger(){
		try{
		if(m_logThread.joinable()){
			m_logThread.join();
		}
		}catch(...){

		}
	}
	bool log(const char* in){
		std::string msg(in);
		return log(msg);
	}
	bool log(std::string &msg){
		auto status = m_msgs.try_push_back(std::move(msg));
		if(status != boost::concurrent::queue_op_status::success){
			return false;
		}
		return true;
	}
private:
	Logger(const std::string &logFile, size_t maxPendingMsgs):m_logFile(logFile), m_msgs(maxPendingMsgs){
			m_logThread = std::thread(&Logger::entry,this);

		}

	    void entry(){
	    	while(true){
	    		try{
	    			logBlocking();
	    		}catch(...){
	    			///error
	    		}
	    	}
	    }
	   void logBlocking(){
		   std::string msg = m_msgs.pull();// blocking
		   m_logFile  << msg<< std::endl << std::flush;
	   }

private:// order of the data members is important
	std::ofstream m_logFile;
	boost::concurrent::sync_bounded_queue<std::string> m_msgs; // not bounded could slow down
	std::thread m_logThread;
};



// test
#include <chrono>

int main()
{
	Logger::Instance().log("msg1");
	std::string msg2("msg1");
	Logger::Instance().log(msg2);
	std::this_thread::sleep_for(std::chrono::seconds(200));// just for test - make sure that main thread is here

	return 0;
}


