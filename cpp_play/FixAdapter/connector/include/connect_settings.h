#ifndef  _CONNECT_SETTINGS_H
#define  _CONNECT_SETTINGS_H

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <utility>

namespace other_fix {

	struct ConnectSettings : private boost::noncopyable {
		ConnectSettings(const std::string& address, const std::string& port);

		const std::string& getAddress() const { return m_address; }
		const std::string& getPort() const { return  m_port; }
		bool getReuseAddress() const { return m_reuseAddress; }
		bool getKeepAlive() const { return m_keepAlives; }
		bool getNoDelay() const { return  m_noDelay; }
		int getRcvedBufferSize() const { return m_ReceivedBufferSize; }
		int getSendBufferSize() const { return m_SendBufferSize; }
		const std::pair<bool, int>& getSoLinger() const { return m_soLinger; }
		int getSoTimeoutMillis() const { return m_soTimeout; }
		int getConnectionIntervaMillis() const { return m_connectionInterval; }
	private:
		std::string        m_address;
		std::string        m_port;
		bool               m_reuseAddress;
		bool               m_keepAlives;
		bool               m_noDelay;
		int                m_ReceivedBufferSize;
		int                m_SendBufferSize;
		std::pair<bool, int> m_soLinger;
		int                  m_soTimeout;
		int                  m_connectionInterval;
	};

	typedef boost::shared_ptr<ConnectSettings> ConnectSettingsPtr;

}  // namespace other_fix

#endif
