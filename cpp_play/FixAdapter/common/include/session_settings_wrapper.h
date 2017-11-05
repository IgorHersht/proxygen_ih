#ifndef _SESSION_SETTINGS_WRAPPER_H
#define _SESSION_SETTINGS_WRAPPER_H
#include <boost/noncopyable.hpp>

#include "fixI.h"
#include "fix_utils.h"
#include "logon_info.h"
#include "logger.h"
#include "logger.h"

namespace other_fix {


	struct  SessionInfo : private boost::noncopyable {

		SessionInfo(const FIX::SessionID & id, const FIX::Dictionary &dict, SessionLogonInfoPtr logonInfo) :
			m_id(id), m_dict(dict), m_logonInfo(logonInfo) {

		}
		const FIX::SessionID& getSessionID() const { return m_id; }
		SessionLogonInfoPtr  getSessionLogonInfo() const { return m_logonInfo; }
		template<typename RT> RT getSettingsValue(const std::string &key) const {
			std::pair<RT, bool> value = getDictionaryValue<RT>(key, m_dict);
			if (!value.second) {
				std::string msg = "Invalid dictionary key:"; msg += key; msg += ":.";
				warningxxx(msg);
			}
			return value.first;
		}
	private:
		const FIX::SessionID    m_id;
		const FIX::Dictionary   &m_dict;
		SessionLogonInfoPtr     m_logonInfo;
	};

	typedef boost::shared_ptr<SessionInfo > SessionInfoPtr;

	struct  SessionsInfo : private boost::noncopyable {

		typedef std::map<FIX::SessionID, SessionInfoPtr>      SessinsSettingsT;
		typedef boost::bimap< FIX::SessionID, TS_SessionIdT > SessionIdsBmT;

		SessionsInfo(const FIX::SessionSettings &settings, FIX::ThreadedSocketAcceptor &acceptor) :
			m_defaul_dict(settings.get()) {
			init(settings, acceptor);
		}

		template<typename RT> RT getSettingsValue(const std::string &key) const {
			std::pair<RT, bool> value = getDictionaryValue<RT>(key, m_defaul_dict);
			if (value.second) {
				return value.first;
			}
			std::string msg = "Invalid dictionary key:"; msg += key; msg += ":.";
			warningxxx(msg);
		}

		template<typename RT> RT getSettingsValue(const std::string &key, const FIX::SessionID& sessionID) const {
			SessinsSettingsT::const_iterator  ses = m_sessinsSettings.find(sessionID);
			if (ses != m_sessinsSettings.end()) {
				return (ses->second)->getSettingsValue<RT>(key);
			}
			getSettingsValue<RT>(key);
		}
		const SessinsSettingsT& getSessionsSettings() const { return m_sessinsSettings; }

		SessionInfoPtr getSessionSettings(const FIX::SessionID &id) const {
			SessinsSettingsT::const_iterator pos = m_sessinsSettings.find(id);
			if (pos != m_sessinsSettings.end()) {
				return pos->second;
			}
			return SessionInfoPtr();
		}
		SessionLogonInfoPtr getLogonInfo(const FIX::SessionID &id) const {
			SessionInfoPtr session = getSessionSettings(id);
			if (!session) {
				return SessionLogonInfoPtr();
			}
			return session->getSessionLogonInfo();

		}
		bool set_sessId(const FIX::SessionID &id, boost::uint_least32_t xsessId, TradingSystemId tsId) const;
		const FIX::SessionID & getSessionId(TS_SessionIdT id) const;

	private:
		void init(const FIX::SessionSettings &settings, FIX::ThreadedSocketAcceptor &acceptor);
		void setSettings(const FIX::SessionSettings &settings, FIX::ThreadedSocketAcceptor &acceptor);
		SessionLogonInfoPtr makeSessionLogonInfoPtr(const FIX::Dictionary& dic, const FIX::SessionID& sessionID) const;
	private:
		const FIX::Dictionary                                  &m_defaul_dict;
		SessinsSettingsT                                       m_sessinsSettings;
		mutable SessionIdsBmT                                  m_sessionIds;// hack
	};



}//namespace other_fix{
#endif
