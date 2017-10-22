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

#include <shared/Message.h>
#include <proxygen/httpserver/HTTPServerAcceptor.h>
#include "ResponseHandler.h"

namespace web_server {
namespace impl {

ResponseHandler::ResponseHandler(proxygen::ResponseHandler* handler,
		proxygen::HTTPServerAcceptor* acceptor) :
		m_handler(handler), m_acceptor(acceptor) {
}

void ResponseHandler::_send() {
	// Once we send them, we don't want to send them again
	SCOPE_EXIT {m_msg.getHeaders().reset();};
#ifdef DEBUG
	m_msg.dump();
#endif

	// By default, chunked
	bool chunked = true;

	// If we have complete response, we can use Content-Length and get done
	if (m_msg.getHeaders() && m_sendEOM) {
		chunked = false;
	}

	if (m_msg.getHeaders()) {
		// We don't need to add Content-Length or Encoding for 1xx responses
		if (m_msg.getHeaders()->getStatusCode() >= 200) {
			if (chunked) {
				m_msg.getHeaders()->setIsChunked(true);
			} else {
				const auto len =
						m_msg.getBody() ?
								m_msg.getBody()->computeChainDataLength() : 0;
				m_msg.getHeaders()->getHeaders().add(
						proxygen::HTTP_HEADER_CONTENT_LENGTH,
						folly::to<std::string>(len));
			}
		}

		m_handler->sendHeaders(*m_msg.getHeaders());
	}

	if (m_msg.getBody()) {
		if (chunked) {
			m_handler->sendChunkHeader(
					m_msg.getBody()->computeChainDataLength());
			m_handler->sendBody(std::move(m_msg.getBody()));
			m_handler->sendChunkTerminator();
		} else {
			m_handler->sendBody(std::move(m_msg.getBody()));
		}
	}

	if (m_sendEOM) {
		m_handler->sendEOM();
	}

}

}
}
