#ifndef _TS_SETTINGS_H
#define _TS_SETTINGS_H

#include "espdI.h"
#include "session_settings_wrapper.h"
#include "connect_settings.h"

#include <boost/lexical_cast.hpp>

#include <sstream>
#include <string>

namespace other_fix {

	struct TradingSytemInfo {

		TradingSytemInfo(
			int connectionInterval,
			int missingHeartBeatThreshold,
			int pingInterval,
			const std::string& name) :
			m_connectionInterval(connectionInterval),
			m_missingHeartBeatThreshold(missingHeartBeatThreshold),
			m_pingInterval(pingInterval),
			m_name(name) {}

		int getConnectionInterval() const { return m_connectionInterval; }
		int getHeartbeatLimit() const { return m_missingHeartBeatThreshold; }
		int getPingInterval() const { return m_pingInterval; }
		const std::string& getName() const { return m_name; }

		std::string to_string() const {
			std::ostringstream info;

			info << "name = " << getName() << ";"
				<< "connection interval = " << getConnectionInterval() << ";"
				<< "heartbeat limit = " << getHeartbeatLimit() << ";"
				<< "ping interval = " << getPingInterval();

			return info.str();
		}

	private:
		int m_connectionInterval;
		int m_missingHeartBeatThreshold;
		int m_pingInterval;
		std::string m_name;
	};

	typedef boost::shared_ptr<TradingSytemInfo> TradingSytemInfoPtr;

	struct TradingSytemSettings {

		TradingSytemSettings(TradingSystemId id, ConnectSettingsPtr connection, TradingSytemInfoPtr info) :
			m_id(id), m_connection(connection), m_info(info) {}

		TradingSystemId getId() const { return m_id; }
		const ConnectSettings& getConnectSettings() const { return *m_connection; }
		const TradingSytemInfo& getTradingSytemInfo() const { return *m_info; }
		std::string to_string() const {
			std::ostringstream os;
			os << getId() << " - TS Info(" << getTradingSytemInfo().to_string() << ")";
			return os.str();
		}

	private:
		TradingSystemId                m_id;
		ConnectSettingsPtr             m_connection;
		TradingSytemInfoPtr            m_info;
	};

	typedef boost::shared_ptr<TradingSytemSettings> TradingSytemSettingsPtr;

	struct TradingSytemsSettings {
		typedef std::map< TradingSystemId, TradingSytemSettingsPtr> TsSettingsT;
		typedef std::map<TradingSystemId, ConnectSettingsPtr > ConnectsSettingsT;

		explicit TradingSytemsSettings(const FIX::SessionSettings& session_setting, const SessionsInfo  &sessionsInfo) :
			m_session_setting(session_setting) {
			init();
		}

		const TsSettingsT& getTsSettings() const { return m_ts_settings; }
		const ConnectsSettingsT& getConnectsSettings() const { return m_connectsSettings; }
		const TradingSystemsId& getTsids() const { return m_tsIds; }
	private:
		void init();
	private:
		const FIX::SessionSettings&                     m_session_setting;
		TsSettingsT                                     m_ts_settings;
		ConnectsSettingsT                               m_connectsSettings;//TODO view not object
		TradingSystemsId                                m_tsIds;
	};

}  //namespace other_fix

#endif
