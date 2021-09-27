/*
 * time_event_generator.cpp
 *
 *  Created on: Mar 25, 2018
 *      Author: ihersht
 */

#include <boost/date_time/posix_time/posix_time.hpp>
#include "time_event_generator.h"


namespace cpp_tools{


TimeEventGeneratorBase::TimeEventGeneratorBase():
	m_context(std::make_shared<boost::asio::io_context>(1)) {
	m_work.push_back(boost::asio::make_work_guard(*m_context));
}

TimeEventGeneratorBase::TimeEventGeneratorBase(std::shared_ptr<boost::asio::io_context >	context):
	m_context(context){
	m_work.push_back(boost::asio::make_work_guard(*m_context));
}

boost::asio::io_context& TimeEventGeneratorBase::get_context(){
	return *m_context;
}


///////////////////////////////////////////////////////////////////


TimeEventGenerator::TimeEventGenerator(std::shared_ptr<boost::asio::io_context >	context):
		TimeEventGeneratorBase(context){
}


void TimeEventGenerator::add_timer(TimeT time, HandlerT handler){
	TimerPtr timer(new boost::asio::deadline_timer(*m_context));
	TimetHandlerT th = std::make_pair(timer, handler);
	{
		std::unique_lock<std::mutex> lc(m_add_mutex);
		m_timers.insert(std::make_pair(time, th));
	}
	timer->expires_at(time);

	timer->async_wait([handler ](const boost::system::error_code& ec){
		if(!ec){
			handler();
		}else{
			// log error
		}
	}
	);

}

void TimeEventGenerator::add_timer(std::string at_utc_time, HandlerT handler){
	TimeT time(boost::posix_time::time_from_string(at_utc_time));
	add_timer(time, handler);
}


void TimeEventGenerator::stop(){
	std::unique_lock<std::mutex> lc(m_stop_mutex);
	if (stopped) {
		return;
	}
	boost::system::error_code ec;
	for(auto & v: m_timers){
		v.second.first->cancel(ec);
		if(ec){
			//log error
		}
	}

}

////

DurationTimeEventGenerator::DurationTimeEventGenerator(std::shared_ptr<boost::asio::io_context >	context):
		TimeEventGeneratorBase(context){
}

void DurationTimeEventGenerator::add_timer(TimeT duration, HandlerT handler){
	TimerPtr timer(new boost::asio::deadline_timer(*m_context));
	TimetHandlerT th = std::make_pair(timer, handler);
	{
		std::unique_lock<std::mutex> lc(m_add_mutex);
		m_timers.insert(std::make_pair(duration, th));
	}

	reset_timer(timer.get(), duration,handler);

}

void DurationTimeEventGenerator::reset_timer(boost::asio::deadline_timer* timer, TimeT duration, HandlerT handler){
	timer->expires_from_now(duration);
	timer->async_wait([this, timer, duration, handler ](const boost::system::error_code& ec){
			if(!ec){
				handler();
				reset_timer(timer, duration,handler);
			}else{
				// log error
			}
		}
		);

}

void DurationTimeEventGenerator::add_timer(size_t ms_duration, HandlerT handler){
	TimeT duration = boost::posix_time::milliseconds(ms_duration);
	add_timer(duration, handler);
}


void DurationTimeEventGenerator::stop(){
	std::unique_lock<std::mutex> lc(m_stop_mutex);
    if(stopped) {
        return;
    }
	boost::system::error_code ec;

	for(auto & v: m_timers){
		v.second.first->cancel(ec);
		if(ec){
			//log error
		}

	}

}





}
