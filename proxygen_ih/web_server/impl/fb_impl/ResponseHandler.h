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
#include <cpp_tools/Logger.h>
#include <shared/Message.h>

namespace proxygen {
class HTTPServerAcceptor;
}
namespace web_server {
namespace impl {

struct ResponseHandler {

	ResponseHandler(proxygen::ResponseHandler* handler,
			proxygen::HTTPServerAcceptor*);

	proxygen::HTTPServerAcceptor* getAcceptor() {
		return m_acceptor;
	}

	shared::Message& getResponseMsg() {
		return m_msg;
	}
	const shared::Message& getResponseMsg() const {
		return m_msg;
	}
	void setResponseMsg(shared::Message&& response) {
		m_msg = std::move(response);
	}
	void closeConnection() {
		m_msg.header(proxygen::HTTP_HEADER_CONNECTION, "close");
	}

	bool copyAndSendWithEOM(const void* buf, uint64_t size) {
		m_msg.appendCopyFromBuffer(buf, size);
		return sendWithEOM();
	}
	;

	bool sendDefaulUserError() {
		m_msg.SetDefaulUserError();
		return sendWithEOM();
	}

	bool sendDefauServerError() {
		m_msg.setDefaulServerError();
		return sendWithEOM();
	}

	void setEOMOnSend(bool sendEOM) {
		m_sendEOM = sendEOM;
	}

	bool sendWithEOM() {
		m_sendEOM = true;
		bool status = send();
		return status;
	}

	bool wasSent() const {
		return m_wasSent;
	}

	bool send() {
		try {
			if (m_wasSent) {
				return true;
			}
			_send();
			m_wasSent = true;
			return true;
		} catch (std::exception &ex) {
			std::string msg = SRC_LOCATION + "exception " + ex.what();
			cpp_tools::log(msg, cpp_tools::ERR);
		} catch (...) {
			std::string msg = SRC_LOCATION + "unknown exception.";
			cpp_tools::log(msg, cpp_tools::ERR);
		}
		return false;
	}

protected:
	void _send();

private:
	proxygen::ResponseHandler* const m_handler { nullptr };
	proxygen::HTTPServerAcceptor* m_acceptor { nullptr };
	shared::Message m_msg;
	// If true, sends EOM.
	bool m_sendEOM { true };bool m_wasSent { false };
};

}
}
