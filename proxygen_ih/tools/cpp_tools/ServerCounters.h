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

#include <iostream>
#include<memory>

#include<string>
#include <atomic>
#include <limits>
#include <folly/Likely.h>

namespace cpp_tools {

enum class CounterId {
	Connection = 0, RegectedConnect, Request, TimeoutedRequest, MaxId
};

template<CounterId id, typename T = uint64_t>
struct Counter {
// staic_asert on T  == int
	Counter() = default;
	void enable() {
		m_enabled = true;
	}

	bool isEnabled() const {
		return m_enabled;
	}

	T get() const noexcept {
		if (!isEnabled()) {
			return 0;
		}
		return m_value;
	}

	T getProcessEstimate(int multiplaer) const noexcept { // assume even distribution
		return m_value * multiplaer;
	}

	void increment() noexcept {
		if (!isEnabled()) {
			return;
		}
		++m_value;
	}

protected:
	T m_value { 0 };bool m_enabled { false };

};

template<CounterId id, typename T = uint64_t>
struct CurrentCounter: public Counter<id, T> {
// staic_asert on T  == int
	CurrentCounter() = default;
	CurrentCounter(T maxValue) :
			m_maxValue(maxValue) {
		this->enable();
	}
	void setMaxValue(T maxValue) noexcept {
		m_maxValue = maxValue;
		this->enable();
	}

	bool isEnabled() const {
		return (m_maxValue < std::numeric_limits<T>::max());
	}

	T geMaxValue() const noexcept {
		if (!isEnabled()) {
			return std::numeric_limits<T>::max();
		}
		return m_maxValue;
	}
	void set(T value) noexcept {
		if (!isEnabled()) {
			return;
		}
		this->m_value = value;
	}

	void decrement() noexcept {
		if (!isEnabled()) {
			return;
		}
		if (LIKELY(this->m_value > 0)) {
			--(this->m_value);
		}
	}
	bool isOverloaded() const noexcept {
		if (!isEnabled()) {
			return false;
		}
		return (this->m_value > m_maxValue);
	}

protected:
	T m_maxValue { std::numeric_limits<T>::max() };

};
/*
 * ratio = dy * 100/dx
 */
template<CounterId XId, CounterId YId, typename T = uint64_t>
struct CurrentRatioCounter {
	CurrentRatioCounter(Counter<XId> &xCounter, Counter<YId> &yCounter) :
			m_xCounter(xCounter), m_yCounter(yCounter) {

	}
	void setMaxValue(T maxRatio) noexcept {
		m_maxRatio = maxRatio;
		if (m_maxRatio < std::numeric_limits<T>::max()) {
			m_xCounter.enable();
			m_yCounter.enable();
		}
	}

	bool isEnabled() const noexcept {
		return (m_maxRatio < std::numeric_limits<T>::max());
	}

	bool isOverloaded() const noexcept {
		if (!isEnabled()) {
			return false;
		}
		return (m_ratio > m_maxRatio);
	}

	void update() {
		if (!isEnabled()) {
			return;
		}
		if (UNLIKELY((m_xCounter.get() - m_previousX) < 0)) {
			return;
		}
		if (UNLIKELY((m_yCounter.get() - m_previousY) < 0)) {
			return;
		}
		const T dx = m_xCounter.get() - m_previousX;
		if (dx < m_interval) {
			return;
		}
		const T dy = m_yCounter.get() - m_previousY;
		m_ratio = dy * 100 / dx;

		m_previousX = m_xCounter.get();
		m_previousY = m_yCounter.get();

		if (isOverloaded()) {
			m_interval = minInterval();
		} else {
			m_interval = maxInterval();
		}

	}

	constexpr static T maxInterval() {
		return 100;
	}
	constexpr static T minInterval() {
		return 5;
	}
protected:
	T m_maxRatio { std::numeric_limits<T>::max() };
	Counter<XId> & m_xCounter;
	Counter<YId> & m_yCounter;
	T m_previousX { 0 };
	T m_previousY { 0 };
	T m_interval { maxInterval() };
	T m_ratio { 0 };
};

typedef Counter<CounterId::Connection> ConnectCounter;
typedef Counter<CounterId::RegectedConnect> RegectedConnectCounter;
typedef CurrentCounter<CounterId::Connection> CurrentConnectCounter;

typedef Counter<CounterId::Request> RequestCounter;
typedef Counter<CounterId::TimeoutedRequest> TimeoutedRequestCounter;
typedef CurrentCounter<CounterId::Request> CurrentRequestCounter;
typedef CurrentRatioCounter<CounterId::Request, CounterId::TimeoutedRequest> TimeoutedRequestRatioCounter;

struct ServerCounters {
	ConnectCounter m_connects;
	RegectedConnectCounter m_regectedConnects;
	CurrentConnectCounter m_currentConnects;

	RequestCounter m_requests;
	TimeoutedRequestCounter m_timeoutedRequests;
	CurrentRequestCounter m_currentRequests;
	TimeoutedRequestRatioCounter m_timeoutedRequestRatio { m_requests,
			m_timeoutedRequests };
};

}
