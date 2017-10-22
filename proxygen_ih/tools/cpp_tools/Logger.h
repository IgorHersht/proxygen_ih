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
#pragma once

#include <string>
#include <boost/thread.hpp>
#include <boost/current_function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <sys/types.h>
#include <unistd.h>
namespace cpp_tools {

namespace _private {

static boost::mutex cout_mutex;
inline void write_to_std_out(const std::string &msg) {
	boost::mutex::scoped_lock lock(cout_mutex);
	try {
		std::cout << msg << std::endl << std::flush;
	} catch (...) {

	}
}
}

const int DBG = -1;
const int INF = 0;
const int WRN = 1;
const int ERR = 2;

inline void default_log(std::string msg, int sev) {
	std::string prefix;
	switch (sev) {
	case DBG: {
		prefix = " DBG-> ";
		break;
	}
	case INF: {
		prefix = " INF-> ";
		break;
	}
	case WRN: {
		prefix = " WARN-> ";
		break;
	}
	case ERR: {
		prefix = " ERR-> ";
		break;
	}
	};
#ifdef DEBUG
	_private::write_to_std_out(prefix + msg);
#else
	if (sev == ERR) {
		_private::write_to_std_out(prefix + msg);
	}
#endif

}

typedef boost::function<void(std::string, int)> logger_type;

inline void log(std::string msg, int sev,
		logger_type loggerInit = default_log) {
	try {
		static logger_type logger = loggerInit;
		if (logger) {
			logger(msg, sev);
		} else {
			//	int i =1;
		}
	} catch (...) {
		// int j = 2;
	}
}


}

#define SRC_LOCATION ( std::string(__FILE__) + ":"  + \
 		boost::lexical_cast<std::string>(__LINE__)  + ":"  + \
 		std::string(__func__) + ":"   )
///std::string(BOOST_CURRENT_FUNCTION) + cpp_tools::LOCATION_SEP )

