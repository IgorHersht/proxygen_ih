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

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <folly/io/IOBuf.h>
#include <cpp_tools/Logger.h>

#include <proxygen/lib/http/HTTPMessage.h>
#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/HTTPServerAcceptor.h>

#include <shared/Message.h>
#include "ResponseHandler.h"
#include "TypesImpl.h"

namespace web_server {
namespace impl {

struct RequestHandlerBase: public proxygen::RequestHandler {
	RequestHandlerBase() {

	}
	~RequestHandlerBase() override;
	void setHTTPServerAcceptor(proxygen::HTTPServerAcceptor* acceptor)
			noexcept override; // assumed called just after ctor

	void setResponseHandler(proxygen::ResponseHandler* handler)
			noexcept override {
		downstream_ = CHECK_NOTNULL(handler);
		m_responseHandler = std::move(
				folly::make_unique<ResponseHandler>(downstream_, m_acceptor));
		m_responseHandler->setEOMOnSend(m_sendEOM);
	}
	void onRequest(std::unique_ptr<proxygen::HTTPMessage> headers)
			noexcept override;
	void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override;
	void onUpgrade(proxygen::UpgradeProtocol) noexcept override {
		// handler doesn't support upgrades
	}
	void requestComplete() noexcept override;
	void onError(proxygen::ProxygenError err) noexcept override;

	void setEOMOnSend(bool sendEOM) {
		m_sendEOM = sendEOM;
	}

	proxygen::HTTPServerAcceptor& getAcceptor() {
		return *m_acceptor;
	}
	shared::Message& getRequestMessage() {
		return m_requestMessage;
	}
	ResponseHandler* getResponseHandler() {
		return m_responseHandler.get();
	}

protected:
	void onTooManyRequests();
	void onTooManyTimeouts();
	void onStopRequested();

protected:
	proxygen::HTTPServerAcceptor* m_acceptor { nullptr };
	shared::Message m_requestMessage;
	ResponseHandlerPtr m_responseHandler;bool m_sendEOM { true };
};

}
}
