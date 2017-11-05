#include "ts_settings.h"
#include "espdI.h"
#include "Const.h"


namespace other_fix {

	void TradingSytemsSettings::init() {

		typedef TsSettingsT::value_type ET;

		try {
			const std::set < FIX::SessionID > ids = m_session_setting.getSessions();
			for (std::set < FIX::SessionID >::const_iterator pos = ids.begin(), end = ids.end(); pos != end; ++pos) {
				const FIX::Dictionary &dic = m_session_setting.get(*pos);
				std::pair<TradingSystemId, bool> ts_id = getDictionaryValue<TradingSystemId>("tsId", dic);
				if (!ts_id.second) {
					warningxxx(pos->toString() + " Innoring invalid TS id. ");
					continue;
				}
				std::pair<std::string, bool> ts_ip = getDictionaryValue<std::string>("TS_IP", dic);
				if (!ts_ip.second) {
					warningxxx(pos->toString() + " Innoring invalid TS id. ");
					continue;
				}
				std::pair<std::string, bool> ts_port = getDictionaryValue<std::string>("TS_PORT", dic);
				if (!ts_port.second) {
					warningxxx(pos->toString() + " Innoring invalid TS id. ");
					continue;
				}
				std::pair<std::string, bool> ts_name = getDictionaryValue<std::string>("tsName", dic);
				if (!ts_name.second) {
					warningxxx(pos->toString() + " Innoring invalid TS name. ");
					ts_name.first = "";
				}

				TradingSytemInfoPtr ts_info(new TradingSytemInfo(consts::DEFAULT_CONNECTION_INTERVAL
					, consts::DEFAULT_MISSING_HEARTBEAT, consts::DEFAULT_PING_INTERVAL, ts_name.first));
				ConnectSettingsPtr connect(new ConnectSettings(ts_ip.first, ts_port.first));
				TradingSytemSettingsPtr ts(new TradingSytemSettings(ts_id.first, connect, ts_info));
				m_ts_settings.insert(ET(ts->getId(), ts));
				m_connectsSettings.insert(ConnectsSettingsT::value_type(ts->getId(), connect));
				m_tsIds.insert(ts->getId());
			}
		}
		catch (std::exception &err) {
			errorxxx(err.what());
		}
		catch (...) {
			errorxxx(" Set sessions info error. ");
		}

	}





}//namespace other_fix{

