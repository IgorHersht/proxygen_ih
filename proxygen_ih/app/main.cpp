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
#include<iostream>
#include<string>
#include <shared/ServerI.h>
#include <shared/ServerManagerI.h>

int main(int argc, char* argv[]) {
	try {
		server::ServerManagerPtr manager =
				server::ServerManager::makeServerManager(argc, argv);
		if (!manager) {
			std::string msg = "Cannot create server manager ";
			std::cerr << msg << std::endl << std::flush;
			return 1;
		}
		server::ServerIPtr server = manager->makeServer();
		if (!server) {
			std::string msg = "Cannot create server ";
			std::cerr << msg << std::endl << std::flush;
			return 1;
		}
		server->start();
	} catch (std::exception &ex) {
		std::string msg = "exception ";
		msg += ex.what();
		std::cerr << msg << std::endl << std::flush;
		return 1;
	} catch (...) {
		std::string msg = "unknown exception.";
		std::cerr << msg << std::endl << std::flush;
		return 1;
	}

	return 0;
}

