

#include <string>
#include <iostream>
#include "signal_handler.h"
#include "session_manager.h"


int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "usage: " << argv[0] << " FILE." << std::endl;
		return 1;
	}
	const  std::string config_file = argv[1];

	other_fix::SessionManagerPtr sessionManager;
	try
	{
		//timing::LatencyItems::getInstance().activate(); // don't remove that line TODO set conditionally
		std::ifstream config_stream(config_file.c_str());
		other_fix::SignalHandler< other_fix::SessionManager> handler;
		sessionManager = other_fix::SessionManagerPtr(new other_fix::SessionManager(config_stream));
		other_fix::SignalHandler< other_fix::SessionManager>::setManager(sessionManager);
		sessionManager->start();
		sessionManager->wait();
		sessionManager->stop();
		return 0;
	}
	catch (std::exception & e) {
		if (sessionManager) {
			sessionManager->stop();
		}
		std::cerr << e.what() << std::endl;
		return 2;
	}
	catch (...) {
		if (sessionManager) {
			sessionManager->stop();
		}
		std::cerr << "Unknown exception." << std::endl;
		return 3;
	}
}

