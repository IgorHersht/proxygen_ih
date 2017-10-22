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

#include <string.h>
#include <exception>
#include <string>
#include <memory>
#include <functional>
#include <type_traits>
#include <boost/lexical_cast.hpp>

#include <glog/logging.h>
#include <folly/ScopeGuard.h>
#include <proxygen/lib/http/HTTPMessage.h>

#include <cpp_tools/Utils.h>
#include "SharedConsts.h"

namespace shared {
typedef proxygen::HTTPMessage Headers;

}

#include <proxygen/lib/http/HTTPMessage.h>

namespace boost {
namespace filesystem {
class path;

}
}

namespace shared {

struct Message {

	typedef std::pair<uint8_t, uint8_t> HTTPVersionT;
	static constexpr HTTPVersionT DefaultHTTPVersion() {
		return HTTPVersionT(1, 1);
	}


	Message(uint16_t statusCode, std::string statusMessage,
			HTTPVersionT version = DefaultHTTPVersion()) :
			m_headers(folly::make_unique<shared::Headers>()) {
		m_headers->setHTTPVersion(version.first, version.second);
		status(statusCode, std::move(statusMessage));
	}

	Message() :
			Message(PROTOCOL_SERVER_ERROR, "") {

	}
	static void SetDefaultUserErrorContent(const std::string &content) {
			s_defaultUserErrorContent = content;
	}
	/// set status
	Message& status(uint16_t code, std::string message) {
		if (!m_headers) {
			m_headers = folly::make_unique<shared::Headers>();
			m_headers->setHTTPVersion(1, 1);
		}
		m_headers->setStatusCode(code);
		m_headers->setStatusMessage(message);
		return *this;
	}

/// headers
	Message& header(std::unique_ptr<Headers> headers) {
		m_headers = std::move(headers);
		return *this;
	}

	const std::unique_ptr<Headers>& getHeaders() const {
		return m_headers;
	}
	std::unique_ptr<Headers>& getHeaders() {
		return m_headers;
	}

	const std::string& getHeaderValue(const std::string& key) const {
		if (!m_headers) {
			return EmptyStr;
		}
		return m_headers->getHeaders().getSingleOrEmpty(key);
	}



	template<typename T> Message& header(proxygen::HTTPHeaderCode code,
			const T& value) {
		if (m_headers) {
			m_headers->getHeaders().add(code, value);
		}
		return *this;
	}

	// add header ( removing possible duplicate)
	template<typename keyT, typename ValueT>
	void setHeader(keyT key, ValueT value) const {
		if (m_headers && (key != keyT())) {
			std::string valueStr;
			bool status = boost::conversion::try_lexical_convert(value,
					valueStr);
			if (status) {
				m_headers->getHeaders().set(key, valueStr);
			}
		}
	}
	// remove all headers which have key
	bool removeHeader(const std::string& key) {
		bool status = false;
		if (m_headers && (!key.empty())) {
			status = m_headers->getHeaders().remove(key);
		}
		return status;
	}

	std::unique_ptr<Headers> moveHeaders() {
		return std::move(m_headers);
	}

/// body
	Message& appendBody(std::unique_ptr<folly::IOBuf>&& bodyIn) {
		if (bodyIn) {
			if (m_body) {
				m_body->prependChain(std::move(bodyIn));
			} else {
				m_body = std::move(bodyIn);
			}
		}
		return *this;
	}

	void appendCopyFromBuffer(const void* buf, uint64_t size) {
		if (buf && (size > 0)) {
			try {
				appendBody(std::move(folly::IOBuf::copyBuffer(buf, size)));
			} catch (...) {

			}
		}
	}

	template<class UniquePtr> void appendTakeOwnership(UniquePtr&& buf,
			size_t count = 1) {
		if (buf) {
			try {
				appendBody(
						std::move(
								folly::IOBuf::takeOwnership(std::move(buf),
										count)));
			} catch (...) {

			}
		}
	}

	// Appends a new IOBuf object that points to an existing user-owned buffer.
	// The user-owned buffer should live longer than IOBuf object (e.g. if the user-owned buffer is static)
	void appendWrapBuffer(const void* buf, uint64_t size) {
		if (buf && (size > 0)) {
			try {
				appendBody(folly::IOBuf::wrapBuffer(buf, size));
			} catch (...) {

			}
		}
	}

	void clearBody() {
		m_body.reset();
	}

	bool appendBodyFromFile(const std::string& filename);bool appendBodyFromExe(
			const std::string &cmd, const boost::filesystem::path &wd);

	const std::unique_ptr<folly::IOBuf>& getBody() const {
		return m_body;
	}
	std::unique_ptr<folly::IOBuf>& getBody() {
		return m_body;
	}

	void SetDefaulUserError() {
		status(PROTOCOL_BAD_REQUEST, "");
		setHeader(proxygen::HTTP_HEADER_CONTENT_TYPE, "text/html");
		setHeader(proxygen::HTTP_HEADER_CONTENT_LENGTH,
				s_defaultUserErrorContent.length());
		auto errBody = folly::IOBuf::copyBuffer(s_defaultUserErrorContent);
		m_body = std::move(errBody);
	}

	void setDefaulServerError() {
		status(PROTOCOL_SERVER_ERROR, "");
		setHeader(proxygen::HTTP_HEADER_CONTENT_TYPE, "text/html");
		clearBody();
	}
	void setServiceUnavailableError() {
		status(PROTOCOL_SERVICE_UNAVAILABLE, "Service Unavailable");
		setHeader(proxygen::HTTP_HEADER_CONTENT_TYPE, "text/html");
		setHeader(proxygen::HTTP_HEADER_CONNECTION, "close");
		clearBody();
	}

	void setTooManyRequestsError() {
		status(PROTOCOL_TOO_MANY_REQUESTS, "Too Many Requests");
		setHeader(proxygen::HTTP_HEADER_CONTENT_TYPE, "text/html");
		setHeader(proxygen::HTTP_HEADER_CONNECTION, "close");
		clearBody();
	}

	void setTimeOutServerError() {
		status(PROTOCOL_REQUEST_TIMEOUT, "TIMEOUT");
		setHeader(proxygen::HTTP_HEADER_CONTENT_TYPE, "text/html");
		clearBody();
	}

	uint16_t getStatus() const {
		if (!m_headers) {
			return PROTOCOL_SERVER_ERROR;
		}
		return m_headers->getStatusCode();
	}

	void dump() const;
private:
	template<typename T>
	static void freeArrayAlocatedWithNew(void* buffer, void*) {
		delete[] static_cast<T*>(buffer);
	}
private:
	static std::string s_defaultUserErrorContent;
	std::unique_ptr<Headers> m_headers;
	std::unique_ptr<folly::IOBuf> m_body;
};

typedef std::unique_ptr<Message> MessagePtr;

}
