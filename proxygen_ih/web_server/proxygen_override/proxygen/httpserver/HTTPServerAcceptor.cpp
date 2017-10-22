/*
 *  Copyright (c) 2017, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#include <proxygen/httpserver/HTTPServerAcceptor.h>

#include <proxygen/httpserver/RequestHandlerAdaptor.h>
#include <proxygen/httpserver/RequestHandlerFactory.h>
#include <proxygen/lib/http/codec/HTTP1xCodec.h>
#include <proxygen/lib/http/codec/HTTP2Constants.h>
#include <proxygen/lib/http/session/HTTPDownstreamSession.h>

#include <folly/futures/InlineExecutor.h>
#include <wangle/concurrent/CPUThreadPoolExecutor.h>

#include <cpp_tools/ServerCounters.h>
#include <shared/OptionDeclarations.h>

using folly::SocketAddress;

namespace proxygen {

AcceptorConfiguration HTTPServerAcceptor::makeConfig(
		const HTTPServer::IPConfig& ipConfig, const HTTPServerOptions& opts) {

	AcceptorConfiguration conf;
	conf.bindAddress = ipConfig.address;
	conf.connectionIdleTimeout = opts.idleTimeout;
	conf.transactionIdleTimeout = opts.idleTimeout;
	conf.initialReceiveWindow = opts.initialReceiveWindow;
	conf.receiveStreamWindowSize = opts.receiveStreamWindowSize;
	conf.receiveSessionWindowSize = opts.receiveSessionWindowSize;
	conf.acceptBacklog = opts.listenBacklog;

	if (ipConfig.protocol == HTTPServer::Protocol::SPDY) {
		conf.plaintextProtocol = "spdy/3.1";
	} else if (ipConfig.protocol == HTTPServer::Protocol::HTTP2) {
		conf.plaintextProtocol = http2::kProtocolCleartextString;
	} else if (opts.h2cEnabled) {
		conf.allowedPlaintextUpgradeProtocols = {http2::kProtocolCleartextString};
	}

	conf.sslContextConfigs = ipConfig.sslConfigs;
	conf.allowInsecureConnectionsOnSecureServer =
			ipConfig.allowInsecureConnectionsOnSecureServer;
	conf.enableTCPFastOpen = ipConfig.enableTCPFastOpen;
	conf.fastOpenQueueSize = ipConfig.fastOpenQueueSize;
	if (ipConfig.ticketSeeds) {
		conf.initialTicketSeeds = *ipConfig.ticketSeeds;
	}
	return conf;
}

std::unique_ptr<HTTPServerAcceptor> HTTPServerAcceptor::make(
		const AcceptorConfiguration& conf, const HTTPServerOptions& opts,
		const std::shared_ptr<HTTPCodecFactory>& codecFactory) {
	// Create a copy of the filter chain in reverse order since we need to create
	// Handlers in that order.
	std::vector<RequestHandlerFactory*> handlerFactories;
	for (auto& f : opts.handlerFactories) {
		handlerFactories.push_back(f.get());
	}
	std::reverse(handlerFactories.begin(), handlerFactories.end());

	return std::unique_ptr<HTTPServerAcceptor>(
			new HTTPServerAcceptor(conf, codecFactory, handlerFactories));
}

HTTPServerAcceptor::HTTPServerAcceptor(const AcceptorConfiguration& conf,
		const std::shared_ptr<HTTPCodecFactory>& codecFactory,
		std::vector<RequestHandlerFactory*> handlerFactories) :
		HTTPSessionAcceptor(conf, codecFactory), handlerFactories_(
				handlerFactories), m_counters(
				folly::make_unique<cpp_tools::ServerCounters>()) {
	setCounters();

	executor_ = folly::make_unique<wangle::CPUThreadPoolExecutor>(
			FLAGS_HTTP_SERVER_CPU_THREADS_PER_IO_THREAD,
			folly::make_unique<
					wangle::LifoSemMPMCQueue<
							wangle::CPUThreadPoolExecutor::CPUTask,
							wangle::QueueBehaviorIfFull::BLOCK> >(
					wangle::CPUThreadPoolExecutor::kDefaultMaxQueueSize),
			std::make_shared<wangle::NamedThreadFactory>("CPUThreadPool"));
	//setGracefulShutdownTimeout(std::chrono::milliseconds(80000));
		//

}

void HTTPServerAcceptor::setCompletionCallback(std::function<void()> f) {
	completionCallback_ = f;
}

cpp_tools::ServerCounters* HTTPServerAcceptor::getCounters() noexcept {
	return m_counters.get();
}

void HTTPServerAcceptor::setCounters() {
	CHECK_NOTNULL(m_counters.get());

	if (FLAGS_HTTP_SERVER_ENABLE_CONNECT_COUNTER) {
		m_counters->m_connects.enable();
	}
	if (FLAGS_HTTP_SERVER_ENABLE_REGECTED_CONNECT_COUNTER) {
		m_counters->m_regectedConnects.enable();
	}
	if (FLAGS_HTTP_SERVER_ENABLE_REQUEST_COUNTER) {
		m_counters->m_requests.enable();
	}
	if (FLAGS_HTTP_SERVER_ENABLE_TIMEOUTED_REQUEST_COUNTER) {
		m_counters->m_timeoutedRequests.enable();
	}

	if (FLAGS_HTTP_SERVER_MAX_CONNECTIONS_PER_IO_THREAD > 0) { // limit set, default no limit
		m_counters->m_currentConnects.setMaxValue(
				FLAGS_HTTP_SERVER_MAX_CONNECTIONS_PER_IO_THREAD);
	}
	if (FLAGS_HTTP_SERVER_MAX_REQUESTS_PER_IO_THREAD > 0) { // limit set, default no limit
		m_counters->m_currentRequests.setMaxValue(
				FLAGS_HTTP_SERVER_MAX_REQUESTS_PER_IO_THREAD);
	}
	if (FLAGS_HTTP_SERVER_MAX_TIMEOUTED_REQUEST_RATIO > 0) {
		m_counters->m_timeoutedRequestRatio.setMaxValue(
				FLAGS_HTTP_SERVER_MAX_TIMEOUTED_REQUEST_RATIO);
	}
}

bool HTTPServerAcceptor::canAccept(const folly::SocketAddress& address) {

	if (UNLIKELY(!m_counters)) {
		return true;
	}

	m_counters->m_currentConnects.set(getNumConnections());

	if (m_counters->m_currentConnects.isOverloaded()) {
		m_counters->m_regectedConnects.increment();
		return false;
	}
	m_counters->m_connects.increment();

	return true;
}

void HTTPServerAcceptor::requestStop() {
	stopRequested_ = true;
	try {
		if (executor_) {
			executor_->stop();
		}
	} catch (...) {

	}
}

HTTPServerAcceptor::~HTTPServerAcceptor() {
}

HTTPTransactionHandler* HTTPServerAcceptor::newHandler(HTTPTransaction& txn,
		HTTPMessage* msg) noexcept {

	SocketAddress clientAddr, vipAddr;
	txn.getPeerAddress(clientAddr);
	txn.getLocalAddress(vipAddr);
	msg->setClientAddress(clientAddr);
	msg->setDstAddress(vipAddr);

	// Create filters chain
	RequestHandler* h = nullptr;
	for (auto& factory : handlerFactories_) {
		h = factory->onRequest(h, msg);
	}

	h->setHTTPServerAcceptor(this);
	RequestHandlerAdaptor* adapter = new RequestHandlerAdaptor(h);
	adapter->setHTTPServerAcceptor(this);
	return adapter;
}

void HTTPServerAcceptor::onConnectionsDrained() {
	if (completionCallback_) {
		completionCallback_();
	}
}

}
