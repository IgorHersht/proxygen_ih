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

#include <fstream>
#include <sys/types.h>
#include <signal.h>
#include <future>
#include <chrono>
#include <map>
#include <memory>
#include <thread>
#include <mutex>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/process.hpp>

namespace cpp_tools {

class ChildProcessReader {

	struct thread_guard {
		thread_guard(boost::thread &&th, ChildProcessReader& reader,
				int64_t maxWaitMlSec) :
				m_th(std::move(th)), m_reader(reader), m_maxWaitMlSec(
						maxWaitMlSec) {
		}
		~thread_guard() {
			clean();
		}
	private:
		void clean() {

			try {
				if (!m_th.joinable()) {
					return;
				}

				if (!m_th.try_join_for(
						boost::chrono::duration<long, boost::milli>(
								m_maxWaitMlSec))) {
					m_reader.kill(10);
					m_th.interrupt();
					m_th.join();
				}

			} catch (...) {

			}

		}
	private:
		boost::thread m_th;
		ChildProcessReader& m_reader;
		int64_t m_maxWaitMlSec = 300000;

	};

public:
// default  maxWaitMlSec 5 min
	static std::string readContent(std::string cmd,
			boost::filesystem::path childWd, int64_t maxWaitMlSec = 300000) {
		static std::mutex chMutex; // only one thread per process can create child process
		std::lock_guard<std::mutex> lk(chMutex);
		std::string content;
		try {

			std::unique_ptr<ChildProcessReader> reader(
					new ChildProcessReader(cmd, childWd));
			{
				if (!reader) {
					return content;
				}
				boost::thread th(start, reader.get());
				thread_guard(std::move(th), *reader, maxWaitMlSec);
			}
			content = reader->getContent();
		} catch (...) {
			content.clear();
		}
		return content;

	}

private:
	static void start(ChildProcessReader *reader) {
		reader->run();
	}

	typedef std::unique_ptr<boost::process::child> ChildPtr;

	ChildProcessReader(const std::string &cmd,
			const boost::filesystem::path &childWd) {
		init(cmd, childWd);
	}
	std::string getContent() const {
		return std::move(m_content);
	}
	bool isRunning() {
		if (!m_child) {
			return false;
		}
		return (::kill(m_child->get_id(), 0) == 0);

	}

	bool kill(int tryMax) {
		for (int tr = 0; tr < tryMax; ++tr) {
			if (kill())
				return true;
		}
		return false;
	}

	bool kill() {
		if (isRunning()) {
			m_child->terminate(false);
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
		if (isRunning()) {
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}

		if (isRunning()) {
			m_child->terminate(true);
			std::this_thread::sleep_for(std::chrono::seconds(4));
		}

		bool status = !isRunning();
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return status;
	}

	void run() noexcept {
		namespace bp = boost::process;
		if (!m_child) {
			return;
		}
		try {
			bp::pistream &is = m_child->get_stdout();
			std::string line;
			while (std::getline(is, line)) {
				m_content += line;
				m_content += "\n";
			}
		} catch (...) {
			m_content.clear();
		}
		clean();
	}

	void clean() {
		try {
			if (m_childWd != m_cwd) {
				boost::filesystem::current_path(m_cwd);
			}
		} catch (...) {
		}
		try {
			if (m_child) {
				m_exitStatus = m_child->wait().exited();
				if (!m_exitStatus) {
					m_content.clear();
				}
			}
		} catch (...) {
			m_content.clear();
		}

	}
	void init(const std::string &cmd, const boost::filesystem::path &childWd)
			noexcept {
		namespace bp = boost::process;
		try {
			m_cwd = boost::filesystem::current_path();
			if (!childWd.empty()) {
				m_childWd = childWd;
				boost::filesystem::current_path(m_childWd);
			} else {
				m_childWd = m_cwd;
			}
			bp::context ctx;
			//ctx.stdin_behavior = bp::inherit_stream();
			ctx.environment = bp::self::get_environment();
			ctx.stdout_behavior = bp::capture_stream();
			ctx.stderr_behavior = bp::silence_stream();
			m_child = std::move(
					ChildPtr(
							new boost::process::child(
									bp::launch_shell(cmd, ctx))));
			if (!isRunning()) {
				m_child.reset();
			}
		} catch (...) {
			m_child.reset();
		}
	}
private:
	boost::filesystem::path m_cwd;
	boost::filesystem::path m_childWd;
	ChildPtr m_child;
	bool m_exitStatus = false;
	std::string m_content;

};

}

