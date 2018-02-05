

#include <memory>
#include <functional>
#include <boost/asio.hpp>
#include <boost/circular_buffer.hpp>


using boost::asio::deadline_timer;

template<typename T> struct WatermarkMessageQueue : public std::enable_shared_from_this< WatermarkMessageQueue <T> >{
	WatermarkMessageQueue(boost::asio::io_context& io_context, size_t high_watermark, size_t low_watermark) :
		m_non_empty_queue(io_context),
		m_queue(high_watermark),
		m_low_watermark(low_watermark) {
		m_non_empty_queue.expires_at(boost::posix_time::pos_infin);
	}

	void start(std::function<void()> on_new_message) {
		await_push(on_new_message);
	}

	void deliver(T&& msg, std::function<void()> on_low_watermark, std::function<void()> on_high_watermark) {
		if (m_queue.full()) {
			on_high_watermark();
			return;
		}
		if (m_queue.size() > low_watermark) {
			on_low_watermar();
			return;
		}

		deliver(std::move(msg));
	}

	void deliver(T && msg) {
		m_queue.push_back(std::move(msg));
		// Signal that the output queue contains messages. Modifying the expiry
		// will wake the output actor, if it is waiting on the timer.
		m_non_empty_queue.expires_at(boost::posix_time::neg_infin);
	}

	void stop() {
		if (m_stopped) {
			return;
		}
		boost::system::error_code ec;
		m_non_empty_queue.cancel(ec);
		m_stopped = true;
		if (!ec) {
			// log error;
		}
		m_queue.clear();
	}

	void await_push(std::function<void()> on_new_message)
	{
		if (stopped)
			return;

		if (m_queue.empty())
		{
			// There are no messages that are ready to be sent. The actor goes to
			// sleep by waiting on the non_empty_output_queue_ timer. When a new
			// message is added, the timer will be modified and the actor will wake.
			m_non_empty_queue.expires_at(boost::posix_time::pos_infin);
			auto self = shared_from_this();
			m_non_empty_queue(async_wait([self] {self->await_push(); }))
		}
		else
		{
			on_new message();
		}
	}

	bool empty() const {
		return m_queue.empty();
	}
	T* front() {
		if (empty()) {
			// log codding error
			return nullptr;
		}
		return &(m_queue.front());
	}
	void pop_front() {
		if (!empty()) {
			m_queue.pop_front();
		}
		else {
			// log codding error
		}
	}

private:
	deadline_timer				m_non_empty_queue;
	boost::circular_buffer<T>	m_queue;
	size_t						m_low_watermark;
	bool						m_stopped{ false };
	
};

//// test
const size_t High_watermark = 10;
const size_t Low_watermark = 5;

int main(int argc, char* argv[])
{
	

	return 0;
}