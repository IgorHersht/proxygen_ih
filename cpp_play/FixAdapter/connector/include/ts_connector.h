#ifndef _TS_CONNECTOR_H
#define _TS_CONNECTOR_H

#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "utils.h"
#include "connect_settings.h"
#include "logger.h"
#include "logger.h"

namespace other_fix {

	struct  TsConnector : private boost::noncopyable {
		typedef boost::asio::ip::tcp tcp;
		typedef boost::asio::mutable_buffer         BufferT;
		typedef boost::asio::mutable_buffers_1      Buffer_1T;
		typedef std::vector<BufferT>                BuffersT;
		explicit  TsConnector(const ConnectSettings &settings) :
			m_settings(settings),
			m_socket(m_io_service),
			m_socketGuard(&m_socket, boost::bind(&tcp::socket::close, boost::ref(m_socket))) {
			m_ipAdress = m_settings.getAddress();
			m_port = m_settings.getPort();
		}

		void connect() {    //therad safe. Will try to connect every connectionInterval. Will not return until OK.
			boost::mutex::scoped_lock lock(m_connect_mutex);
			connect_not_locked();
		}

		void disconnect() {    //therad safe
			boost::mutex::scoped_lock lock(m_connect_mutex);
			disconnect_not_locked();
		}

		void send_to_ts(const BuffersT &bufs) { // can be called from sender thread only
			boost::system::error_code  ec = boost::asio::error::bad_descriptor;
			try {
				boost::asio::write(m_socket, bufs, ec);
				if (ec) {
					std::string msg = "An error on socket::write. "; msg += ec.message();
					warningxxx(msg);
					connect();
					send_to_ts(bufs);
				}
			}
			catch (...) {
				std::string msg = "An error on socket::write.";
				warningxxx(msg);
				disconnect();
				throw;
			}
		}

		void  receive_from_ts(Buffer_1T buf) {  // can be called from receiver thread only
			boost::system::error_code  ec = boost::asio::error::bad_descriptor;
			try {
				boost::asio::read(m_socket, buf, ec);
				if (ec) {
					std::string msg = "An error on socket::read. "; msg += ec.message();
					warningxxx(msg);
					connect();
					receive_from_ts(buf);
				}
			}
			catch (...) {
				std::string msg = "An error on socket::read.";
				warningxxx(msg);
				disconnect();
				throw;
			}

		}

	private:
		enum PrependSeparator { DONT_PREPEND, PREPEND };
		void connect_not_locked();
		void connect_not_locked(tcp::resolver::iterator endpoint_iterator, boost::system::error_code &error);
		std::string to_string() const;
		bool connected() const;
		void setOptions();
		void disconnect_not_locked();

		template <class T> void set_socket_option(const T& value, const char* option_description)
		{
			try {
				boost::system::error_code error = boost::asio::error::no_protocol_option;
				m_socket.set_option(value, error);
				if (error) {
					std::string msg = "Cannot set "; msg += option_description;
					msg += ". Error: ";       msg += error.message();
					msg += " Default value for the option is going to be used";
					errorxxx(msg);
				}
			}
			catch (...) {
				std::string msg = "General error occurred while setting socket option ";
				msg += option_description;
				errorxxx(std::string("General error occurred: ") + msg);
			}
		}

		template <class T> void record_current_socket_option(T& option, const char* option_description,
			PrependSeparator prepend_separator, std::ostringstream& stream) const {

			if (prepend_separator == PREPEND) stream << "; ";
			boost::system::error_code ec;
			m_socket.get_option(option, ec);
			stream << option_description << " = " << option.value();
		}

	private:
		const ConnectSettings                                       &m_settings;
		boost::asio::io_service                                     m_io_service;
		tcp::socket                                                 m_socket;
		boost::shared_ptr<tcp::socket>                              m_socketGuard;
		boost::mutex                                                m_connect_mutex;
		std::string                                                 m_ipAdress;
		std::string                                                 m_port;
	};

}  // namespace other_fix
#endif
