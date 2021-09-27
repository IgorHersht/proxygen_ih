/*
 * time_event_generator.h
 *
 *  Created on: Mar 25, 2018
 *      Author: ihersht
 */

#pragma once
#include<functional>
#include<memory>
#include<map>
#include<list>
#include<boost/asio.hpp>

namespace cpp_tools{

struct TimeEventGeneratorBase {
	typedef std::function<void(void)> HandlerT;
	typedef std::shared_ptr<boost::asio::deadline_timer> TimerPtr;
	typedef std::pair<TimerPtr,HandlerT > TimetHandlerT;

	TimeEventGeneratorBase();
	explicit TimeEventGeneratorBase(std::shared_ptr<boost::asio::io_context >	context);

	boost::asio::io_context& get_context();

protected:
	typedef boost::asio::executor_work_guard<boost::asio::io_context::executor_type> io_context_work;
	std::shared_ptr<boost::asio::io_context >	m_context;
	std::list<io_context_work> 					m_work;
	std::mutex 									m_add_mutex;
	std::mutex 									m_stop_mutex;
	bool										m_stopped{ false };

};


struct TimeEventGenerator : public TimeEventGeneratorBase {

typedef boost::posix_time::ptime 		 TimeT;
typedef std::map<TimeT,TimetHandlerT >   TimersT;

explicit TimeEventGenerator(std::shared_ptr<boost::asio::io_context >	context);

void add_timer(TimeT time, HandlerT handler);// execute handler one time at time if not stopped before. Must be called before start();
void add_timer(std::string at_utc_time, HandlerT handler);// e.g. "2018-03-05 16:34:00.000"
void stop();

private:
	TimersT  									m_timers;
};

struct DurationTimeEventGenerator : public TimeEventGeneratorBase{

typedef boost::posix_time::time_duration TimeT;
typedef std::map<TimeT,TimetHandlerT > TimersT;

explicit DurationTimeEventGenerator(std::shared_ptr<boost::asio::io_context >	context);

void add_timer(TimeT duration, HandlerT handler);// execute handler every duration until stopped. Must be called before start();
void add_timer(size_t ms_duration, HandlerT handler);// duration in milliseconds

void stop();
private:
void reset_timer(boost::asio::deadline_timer* timer, TimeT duration, HandlerT handler);
private:
	TimersT								m_timers;


};






}


