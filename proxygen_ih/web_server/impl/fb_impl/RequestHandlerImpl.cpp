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

#include <wangle/concurrent/CPUThreadPoolExecutor.h>
#include <cpp_tools/ServerCounters.h>
#include <shared/App2WebI.h>

#include "RequestHandlerImpl.h"

namespace web_server {
namespace impl {

///////////////////
void ThreadFanctor::operator()() noexcept {
	if (UNLIKELY(!m_requestHandler)) {
		std::string msg = SRC_LOCATION + "m_requestHandler is NULL ";
		cpp_tools::log(msg, cpp_tools::ERR);
		return;
	}
	shared::Message responseMessage;
	responseMessage.setDefaulServerError();
	try {
		responseMessage = std::move(
				web_server_app_ns::makeResponse(
						m_requestHandler->getRequestMessage()));
	} catch (std::exception &ex) {
		std::string msg = SRC_LOCATION + "exception " + ex.what();
		cpp_tools::log(msg, cpp_tools::ERR);
	} catch (...) {
		std::string msg = SRC_LOCATION + "unknown exception.";
		cpp_tools::log(msg, cpp_tools::ERR);
	}

	bool status = sendResponseNoexcept(responseMessage);
	if (!status) {
		std::string msg = SRC_LOCATION + " sendResponse problems";
		cpp_tools::log(msg, cpp_tools::ERR);
	}
}

void ThreadFanctor::onAppTimeout() noexcept {
	shared::Message responseMessage;
	responseMessage.setTimeOutServerError();
	bool status = sendResponseNoexcept(responseMessage);
	if (!status) {
		std::string msg = SRC_LOCATION + " sendResponse problems";
		cpp_tools::log(msg, cpp_tools::ERR);
	}
}

static void sendFunc(ResponseHandler* responseHandler) {

	try {
		responseHandler->send();
	} catch (std::exception &ex) {
		std::string msg = SRC_LOCATION + "exception " + ex.what();
		cpp_tools::log(msg, cpp_tools::ERR);
	} catch (...) {
		std::string msg = SRC_LOCATION + "unknown exception.";
		cpp_tools::log(msg, cpp_tools::ERR);
	}
}

bool ThreadFanctor::sendResponse(shared::Message &responseMessage) {
	folly::EventBase* eb = m_requestHandler->getAcceptor().getEventBase();
	if (UNLIKELY(!eb)) {
		std::string msg = SRC_LOCATION + "folly::EventBase* is NULL ";
		cpp_tools::log(msg, cpp_tools::ERR);
		return false;
	}
	ResponseHandler* responseHandler = m_requestHandler->getResponseHandler();
	if (UNLIKELY(!responseHandler)) {
		std::string msg = SRC_LOCATION + "responseHandler is NULL ";
		cpp_tools::log(msg, cpp_tools::ERR);
		return false;
	}
	responseHandler->setResponseMsg(std::move(responseMessage));

	bool status = eb->runInEventBaseThread(sendFunc, responseHandler);
	if (!status) {
		std::string msg = SRC_LOCATION + "cannot send response ";
		cpp_tools::log(msg, cpp_tools::ERR);
		return false;
	}

	return true;
}

bool ThreadFanctor::sendResponseNoexcept(shared::Message &responseMessage)
		noexcept {
	try {
		bool status = sendResponse(responseMessage);
		return status;
	} catch (std::exception &ex) {
		std::string msg = SRC_LOCATION + "exception " + ex.what();
		cpp_tools::log(msg, cpp_tools::ERR);
	} catch (...) {
		std::string msg = SRC_LOCATION + "unknown exception.";
		cpp_tools::log(msg, cpp_tools::ERR);
	}
	return false;
}

void AsyncRequestHandlerImpl::onEOM() noexcept {
	if (UNLIKELY(!m_acceptor)) {
		std::string msg = SRC_LOCATION + "acceptor is NULL";
		cpp_tools::log(msg, cpp_tools::ERR);
		return;
	}
	try {
		if (m_acceptor->isStopRequested()) {
			onStopRequested();
			return;
		}
		if (m_acceptor->getCounters()) {

			if (m_acceptor->getCounters()->m_currentRequests.isOverloaded()) {
				onTooManyRequests();
				return;
			}
			if (m_acceptor->getCounters()->m_timeoutedRequestRatio.isOverloaded()) {
				onTooManyTimeouts();
				return;
			}
		}

		m_threadFanctor.set(this);
		auto* executor = m_acceptor->getExecutor();
		if (UNLIKELY(!executor)) {
			std::string msg = SRC_LOCATION + "executor is NULL";
			cpp_tools::log(msg, cpp_tools::ERR);
			return;
		}

		const ulong timeOut = web_server_app_ns::getTimeOut(m_requestMessage);
		if (timeOut > 0) {
			ThreadFanctor * threadFanctorPtr = &m_threadFanctor;
			executor->add(m_threadFanctor, std::chrono::milliseconds(timeOut),
					[threadFanctorPtr]() {threadFanctorPtr->onAppTimeout();});
		} else {
			executor->add(m_threadFanctor);
		}
		return;
	} catch (wangle::QueueFullException &ex) {
		std::string msg = SRC_LOCATION + "exception " + ex.what();
		cpp_tools::log(msg, cpp_tools::ERR);
		try {
			onTooManyRequests();
		} catch (...) {
		}
		return;
	} catch (std::exception &ex) {
		std::string msg = SRC_LOCATION + "exception " + ex.what();
		cpp_tools::log(msg, cpp_tools::ERR);
	} catch (...) {
		std::string msg = SRC_LOCATION + "unknown exception.";
		cpp_tools::log(msg, cpp_tools::ERR);
	}
	try {
		m_responseHandler->sendDefauServerError();
	} catch (...) {

	}
}

}
}

