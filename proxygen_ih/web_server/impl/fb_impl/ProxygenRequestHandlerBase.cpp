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

#include <folly/Memory.h>
#include <folly/Likely.h>
#include <cpp_tools/ServerCounters.h>
#include <shared/App2WebI.h>
#include <proxygen/httpserver/HTTPServer.h>
#include "ProxygenRequestHandlerBase.h"

namespace web_server {
namespace impl {

void RequestHandlerBase::setHTTPServerAcceptor(
		proxygen::HTTPServerAcceptor* acceptor) noexcept {
	m_acceptor = CHECK_NOTNULL(acceptor);

	m_acceptor->getCounters()->m_requests.increment();
	m_acceptor->getCounters()->m_currentRequests.increment();
	m_acceptor->getCounters()->m_timeoutedRequestRatio.update();
}

RequestHandlerBase::~RequestHandlerBase() {
	if (LIKELY((m_acceptor != nullptr) && (m_acceptor->getCounters()))) {
		m_acceptor->getCounters()->m_currentRequests.decrement();
	}
}

void RequestHandlerBase::onRequest(
		std::unique_ptr<proxygen::HTTPMessage> headers) noexcept {
	m_requestMessage.header(std::move(headers));
}

void RequestHandlerBase::onBody(std::unique_ptr<folly::IOBuf> body) noexcept {
	m_requestMessage.appendBody(std::move(body));
}

void RequestHandlerBase::onTooManyRequests() {

	shared::Message responseMessage;
	responseMessage.setTooManyRequestsError();
	m_responseHandler->setResponseMsg(std::move(responseMessage));
	m_responseHandler->sendWithEOM();
}

void RequestHandlerBase::onStopRequested() {
	shared::Message responseMessage;
	responseMessage.setServiceUnavailableError();
	m_responseHandler->setResponseMsg(std::move(responseMessage));
	m_responseHandler->sendWithEOM();

}
void RequestHandlerBase::onTooManyTimeouts() {
	onTooManyRequests();
}

void RequestHandlerBase::requestComplete() noexcept {
	delete this;
}

void RequestHandlerBase::onError(proxygen::ProxygenError err) noexcept {
	delete this;
}

}
}
