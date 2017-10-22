/*
 *  Copyright (c) 2017, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#pragma once
#include <gflags/gflags.h>
#include <proxygen/httpserver/HTTPServer.h>
#include <proxygen/httpserver/HTTPServerOptions.h>
#include <proxygen/lib/http/session/HTTPSessionAcceptor.h>

namespace cpp_tools {
struct ServerCounters;
}

namespace proxygen {

class HTTPServerAcceptor final : public HTTPSessionAcceptor {
public:
	static AcceptorConfiguration makeConfig(
			const HTTPServer::IPConfig& ipConfig,
			const HTTPServerOptions& opts);

	static std::unique_ptr<HTTPServerAcceptor> make(
			const AcceptorConfiguration& conf, const HTTPServerOptions& opts,
			const std::shared_ptr<HTTPCodecFactory>& codecFactory = nullptr);

	/**
	 * Invokes the given method when all the connections are drained
	 */
	void setCompletionCallback(std::function<void()> f);

	~HTTPServerAcceptor() override;

	wangle::ThreadPoolExecutor* getExecutor() {
		return executor_.get();
	}
	bool canAccept(const folly::SocketAddress& address) override;
	void requestStop();bool isStopRequested() const {
		return stopRequested_;
	}

	cpp_tools::ServerCounters* getCounters() noexcept;

private:
	HTTPServerAcceptor(const AcceptorConfiguration& conf,
			const std::shared_ptr<HTTPCodecFactory>& codecFactory,
			std::vector<RequestHandlerFactory*> handlerFactories);

	// HTTPSessionAcceptor
	HTTPTransaction::Handler* newHandler(HTTPTransaction& txn, HTTPMessage* msg)
			noexcept override;
	void onConnectionsDrained() override;
	void setCounters();

	std::function<void()> completionCallback_;
	const std::vector<RequestHandlerFactory*> handlerFactories_ { nullptr };
	std::unique_ptr<cpp_tools::ServerCounters> m_counters;
	std::unique_ptr<wangle::ThreadPoolExecutor> executor_;bool stopRequested_ {
			false };

};

}
