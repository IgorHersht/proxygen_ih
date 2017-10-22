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
#include <fstream>
#include <boost/filesystem.hpp>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <folly/FileUtil.h>
#include <folly/FBString.h>

#include <cpp_tools/ChildProcessReader.h>

#include <shared/Options.h>
#include <shared/Message.h>

namespace shared {
std::string Message::s_defaultUserErrorContent;

void Message::dump() const {
	try {
		if (!m_headers) {
			return;
		}
		if (m_headers->isRequest()) {
			VLOG(1) << "Start  request dump *******";
		} else if (m_headers->isResponse()) {
			VLOG(1) << "Start  response dump ******";
		}
		VLOG(1) << "Dumping header";
		m_headers->atomicDumpMessage(3);
		VLOG(1) << "Dumping body";
		if (!m_body || m_body->empty()) {
			VLOG(1) << "Empty body";
		} else {
			std::string bodyStr((const char*) m_body->data(), m_body->length());
			VLOG(1) << bodyStr;
		}
		VLOG(1) << "End dump ******";
		google::FlushLogFiles(google::GLOG_INFO);

	} catch (...) {

	}
}

// read the contents of the file
bool Message::appendBodyFromFile(const std::string& filename) {

	try {
		std::string content;
		if (!folly::readFile(filename.c_str(), content)) { // Shame on me - extra copy here
			return false;
		}
		if (!(content.empty())) {
			appendBody(
					folly::IOBuf::copyBuffer(content.data(), content.length()));
		}
		return true;
	} catch (...) {
		return false;
	}
}

bool Message::appendBodyFromExe(const std::string &cmd,
		const boost::filesystem::path &wd) {

	std::string content = cpp_tools::ChildProcessReader::readContent(cmd, wd,
			FLAGS_HTTP_SERVER_CHILD_TIMEOUT);
	if (!(content.empty())) {
		appendBody(folly::IOBuf::copyBuffer(content.data(), content.length()));
		return true;
	} else {
		return false;
	}

}

}
