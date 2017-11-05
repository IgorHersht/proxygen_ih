
#include "session_settings_wrapper.h"

namespace other_fix {

	void SessionsInfo::init(const FIX::SessionSettings &settings, FIX::ThreadedSocketAcceptor &acceptor) {
		try {
			setSettings(settings, acceptor);
		}
		catch (...) {
			throw;
		}
	}

	void SessionsInfo::setSettings(const FIX::SessionSettings &settings, FIX::ThreadedSocketAcceptor &acceptor) {

		const std::set < FIX::SessionID > ids = settings.getSessions();
		for (std::set < FIX::SessionID >::const_iterator pos = ids.begin(), end = ids.end(); pos != end; ++pos) {
			try {
				const FIX::Dictionary &dic = settings.get(*pos);
				FIX::Session* ses = acceptor.getSession(*pos);
				if (!ses) {
					throw FIXError(pos->toString() + " Invalid sesion id. ");
				}
				SessionLogonInfoPtr logonInfo = makeSessionLogonInfoPtr(dic, *pos);
				SessionInfoPtr wr(new SessionInfo(*pos, dic, logonInfo));
				m_sessinsSettings.insert(SessinsSettingsT::value_type(*pos, wr));
			}
			catch (std::exception &err) {
				errorxxx(err.what());
			}
			catch (...) {
				errorxxx(" Set sessions info error. ");
			}
		}
	}

	SessionLogonInfoPtr SessionsInfo::makeSessionLogonInfoPtr(const FIX::Dictionary& dic, const FIX::SessionID& sessionID) const {
		return SessionLogonInfoPtr(new SessionLogonInfo(
			boost::lexical_cast<boost::uint_least32_t>(dic.getString("tsPref")),
			sessionID.toString(), dic.getString("userName"), dic.getString("passWord"),
			boost::lexical_cast<boost::uint_least32_t>(dic.getString("tsId"))
		));

	}

	bool SessionsInfo::set_sessId(const FIX::SessionID &id, boost::uint_least32_t xsessId, boost::uint_least32_t tsId)const {
		SessionLogonInfoPtr info = getLogonInfo(id);
		if (!info) {
			return false;
		}
		if (tsId != info->get_tsId()) {
			return false;
		}

		info->set_xsessId(xsessId);
		m_sessionIds.insert(SessionIdsBmT::value_type(id, TS_SessionIdT(info->get_xsessId(info->get_tsId()), info->get_tsId())));
		return true;
	}

	const FIX::SessionID & SessionsInfo::getSessionId(TS_SessionIdT id) const {
		typedef SessionIdsBmT::right_map MapT;
		const MapT &map = m_sessionIds.right;

		MapT::const_iterator pos = map.find(id);
		if (pos != map.end()) {
			return pos->second;
		}
		return fix_consts::INVALID_SESSION_ID;
	}

}//namespace other_fix{

