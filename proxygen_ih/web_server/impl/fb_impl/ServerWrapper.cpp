
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

#include <stdexcept>
#include <gflags/gflags.h>
#include <folly/Memory.h>
#include <folly/Portability.h>
#include <folly/io/async/EventBaseManager.h>
#include <proxygen/httpserver/HTTPServer.h>
#include <shared/Options.h>
#include <shared/ServerI.h>
#include "RequestHandlerFactoryImpl.h"
#include "ServerWrapper.h"
#include "TypesImpl.h"

namespace web_server {
namespace impl {

ServerWrapper::ServerWrapper() {

	std::vector<ImplT::IPConfig> IPs = { { folly::SocketAddress(
			FLAGS_HTTP_SERVER_IP, FLAGS_HTTP_SERVER_PORT, true),
			ImplT::Protocol::HTTP }
	//, { folly::SocketAddress("127.0.0.1", FLAGS_HTTP_SERVER_PORT, true), proxygen::HTTPServer::Protocol::HTTP}
			};
	int thread_num = FLAGS_HTTP_SERVER_IO_THREADS;
	if (thread_num <= 0) {
		thread_num = sysconf(_SC_NPROCESSORS_ONLN);
		// CHECK(FLAGS_threads > 0);
	}

	proxygen::HTTPServerOptions httpOptions;

	httpOptions.threads = static_cast<size_t>(thread_num);
	httpOptions.shutdownOn = {SIGINT, SIGTERM};
	httpOptions.idleTimeout = std::chrono::milliseconds(
			FLAGS_HTTP_SERVER_IDLE_TIMEOUT);
	httpOptions.listenBacklog = FLAGS_HTTP_SERVER_LISTEN_BACKLOG;
	httpOptions.enableContentCompression = false;
	httpOptions.supportsConnect = FLAGS_HTTP_SERVER_SUPPORTS_CONNECT;

	httpOptions.handlerFactories = proxygen::RequestHandlerChain().addThen<
			RequestHandlerFactoryImpl>().build();

	m_impl = std::make_shared<ImplT>(std::move(httpOptions));
	m_impl->bind(IPs);

}

void ServerWrapper::start() { // looks like all abnormal conditions handled in the impl
	std::thread t([&] () {
		m_impl->start();
	});

	t.join();
}
void ServerWrapper::stop() {
// Need to stop? but need to do it from the server thread.
}

}
}

