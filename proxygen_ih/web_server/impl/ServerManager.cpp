/*
MIT License

Copyright @ 2017 Igor Hersht

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <exception>
#include <string>
#include <map>
#include <memory>
#include <functional>
#include <boost/filesystem.hpp>
#include <gflags/gflags.h>
#include <glog/logging.h>

#include <cpp_tools/Utils.h>
#include <cpp_tools/Logger.h>
#include <shared/App2WebI.h>
#include <shared/Options.h>
#include <shared/SharedConsts.h>
#include <shared/ServerI.h>
#include <shared/ServerManagerI.h>

namespace server {
static void failureWriter(const char* data, int size) {
	std::string msg(data, size);
	cpp_tools::log(msg, cpp_tools::ERR);
}

ServerManagerPtr ServerManager::makeServerManager(int argc, char* argv[]) {
	init(argc, argv);
	return std::make_shared<ServerManager>();
}

ServerManager::ServerManager() {
}

ServerIPtr ServerManager::makeServer() {
	return std::make_shared<ServerI>();
}

void ServerManager::init(int argc, char* argv[]) {
	initFailureSignalHandler();
	setAppMane(argc, argv);
	initOptions(argc, argv);
	initImpl();
	initLogger(argv[0]);
}

void ServerManager::initFailureSignalHandler() {
	signal(SIGPIPE, SIG_IGN);
	google::InstallFailureSignalHandler();
}

void ServerManager::initOptions(int argc, char* argv[]) {
	if ((argc > 1) && argv[1]) {
		parseConfig(argv[1]);
	} else {
		std::string msg = SRC_LOCATION
				+ "Config file should be first on command line";
		throw std::runtime_error(msg);
	}
	parseCmdLine(argc, argv);
}
void ServerManager::parseConfig(const char* cfgFile) {
	if (!web_server_app_ns::parseConfigFile(cfgFile)) {
		std::string msg = SRC_LOCATION + "Cannot parse config file ";
		throw std::runtime_error(msg);
	}
	FLAGS_HTTP_SERVER_IP = shared::findConfigValue("HTTP_SERVER_IP",
			FLAGS_HTTP_SERVER_IP);
	FLAGS_HTTP_SERVER_PORT = shared::findConfigValue("HTTP_SERVER_PORT",
			FLAGS_HTTP_SERVER_PORT);

	FLAGS_HTTP_SERVER_IDLE_TIMEOUT = shared::findConfigValue(
			"HTTP_SERVER_IDLE_TIMEOUT", FLAGS_HTTP_SERVER_IDLE_TIMEOUT);
	FLAGS_HTTP_SERVER_CHILD_TIMEOUT = shared::findConfigValue(
			"HTTP_SERVER_CHILD_TIMEOUT", FLAGS_HTTP_SERVER_IDLE_TIMEOUT);
	FLAGS_HTTP_SERVER_LISTEN_BACKLOG = shared::findConfigValue(
			"HTTP_SERVER_LISTEN_BACKLOG", FLAGS_HTTP_SERVER_LISTEN_BACKLOG);
	FLAGS_HTTP_SERVER_SUPPORTS_CONNECT = shared::findConfigValue(
			"HTTP_SERVER_SUPPORTS_CONNECT", FLAGS_HTTP_SERVER_SUPPORTS_CONNECT);
	FLAGS_HTTP_SERVER_WD = shared::findConfigValueNoThrow("HTTP_SERVER_WD",
			FLAGS_HTTP_SERVER_WD);

	FLAGS_HTTP_SERVER_IO_THREADS = shared::findConfigValue(
			"HTTP_SERVER_IO_THREADS", FLAGS_HTTP_SERVER_IO_THREADS);
	FLAGS_HTTP_SERVER_CPU_THREADS_PER_IO_THREAD = shared::findConfigValue(
			"HTTP_SERVER_CPU_THREADS_PER_IO_THREAD",
			FLAGS_HTTP_SERVER_CPU_THREADS_PER_IO_THREAD);


	FLAGS_HTTP_SERVER_TIMEOUT_REQUST = shared::findConfigValue(
			"HTTP_SERVER_TIMEOUT_REQUST", FLAGS_HTTP_SERVER_TIMEOUT_REQUST);
	FLAGS_HTTP_SERVER_TIMEOUT_REQUST_CODE = shared::findConfigValue(
			"HTTP_SERVER_TIMEOUT_REQUST_CODE",
			FLAGS_HTTP_SERVER_TIMEOUT_REQUST_CODE);

	FLAGS_HTTP_SERVER_ENABLE_CONNECT_COUNTER = shared::findConfigValue(
			"HTTP_SERVER_ENABLE_CONNECT_COUNTER",
			FLAGS_HTTP_SERVER_ENABLE_CONNECT_COUNTER);
	FLAGS_HTTP_SERVER_ENABLE_REGECTED_CONNECT_COUNTER = shared::findConfigValue(
			"HTTP_SERVER_ENABLE_REGECTED_CONNECT_COUNTER",
			FLAGS_HTTP_SERVER_ENABLE_REGECTED_CONNECT_COUNTER);
	FLAGS_HTTP_SERVER_ENABLE_REQUEST_COUNTER = shared::findConfigValue(
			"HTTP_SERVER_ENABLE_REQUEST_COUNTER",
			FLAGS_HTTP_SERVER_ENABLE_REQUEST_COUNTER);
	FLAGS_HTTP_SERVER_ENABLE_TIMEOUTED_REQUEST_COUNTER =
			shared::findConfigValue(
					"HTTP_SERVER_ENABLE_TIMEOUTED_REQUEST_COUNTER",
					FLAGS_HTTP_SERVER_ENABLE_TIMEOUTED_REQUEST_COUNTER);

	FLAGS_HTTP_SERVER_MAX_CONNECTIONS_PER_IO_THREAD = shared::findConfigValue(
			"HTTP_SERVER_MAX_CONNECTIONS_PER_IO_THREAD",
			FLAGS_HTTP_SERVER_MAX_CONNECTIONS_PER_IO_THREAD);
	FLAGS_HTTP_SERVER_MAX_REQUESTS_PER_IO_THREAD = shared::findConfigValue(
			"HTTP_SERVER_MAX_REQUESTS_PER_IO_THREAD",
			FLAGS_HTTP_SERVER_MAX_REQUESTS_PER_IO_THREAD);
	FLAGS_HTTP_SERVER_MAX_TIMEOUTED_REQUEST_RATIO = shared::findConfigValue(
			"HTTP_SERVER_MAX_TIMEOUTED_REQUEST_RATIO",
			FLAGS_HTTP_SERVER_MAX_TIMEOUTED_REQUEST_RATIO);

}

void ServerManager::setAppMane(int argc, char* argv[]) {
	boost::filesystem::path app_path = boost::filesystem::path(argv[0]);
	std::string name = boost::filesystem::absolute(app_path).string();
	FLAGS_HTTP_SERVER_APPLICATION_NAME = name;
}

void ServerManager::parseCmdLine(int argc, char* argv[]) {
	gflags::ParseCommandLineFlags(&argc, &argv, true);
	//google::ParseCommandLineFlags(&argc, &argv, true);

}
void ServerManager::initImpl() {
	if (!(FLAGS_HTTP_SERVER_WD.empty())) {
		if (!cpp_tools::setWD(FLAGS_HTTP_SERVER_WD)) {
			std::string msg = SRC_LOCATION
					+ "Cannot change working directory to "
					+ FLAGS_HTTP_SERVER_WD;
			std::cerr << msg << std::endl << std::flush;
		}
	}
	int status = web_server_app_ns::impl_init();
	if (status == REQ_ABORTED) {
		std::string msg = SRC_LOCATION + "Cannot init application";
		throw std::runtime_error(msg);
	}
}
void ServerManager::initLogger(const char* appName) {
	google::InitGoogleLogging(appName);
	FLAGS_log_dir = web_server_app_ns::findLogDir();
	std::string msg = SRC_LOCATION + " Web server logger initialized  ";
	cpp_tools::log(msg, cpp_tools::INF, web_server_app_ns::logger);
#ifdef DEBUG2
	FLAGS_minloglevel = google::INFO;
	FLAGS_v = 15;
#else
	google::InstallFailureWriter(failureWriter);
#endif

}

}

