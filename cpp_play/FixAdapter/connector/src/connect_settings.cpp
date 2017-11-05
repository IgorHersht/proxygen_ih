#include "connect_settings.h"

#include "Const.h"
#include "utils.h"
#include "logger.h"


namespace other_fix {

	ConnectSettings::ConnectSettings(const std::string& address, const std::string& port) :
		m_address(address),
		m_port(port),
		m_reuseAddress(consts::DEFAULT_SO_REUSE_ADDR),
		m_keepAlives(consts::DEFAULT_SO_KEEPALIVE),
		m_noDelay(consts::DEFAULT_SO_NODELAY),
		m_ReceivedBufferSize(consts::DEFAULT_RCV_BUFFER_SIZE),
		m_SendBufferSize(consts::DEFAULT_SND_BUFFER_SIZE),
		m_soLinger(std::pair<bool, int>(true, consts::DEFAULT_SO_LINGER_TIME)),
		m_soTimeout(consts::DEFAULT_SO_TIMEOUT),
		m_connectionInterval(consts::DEFAULT_CONNECTION_INTERVAL) {


	}

}  // namespace other_fix

