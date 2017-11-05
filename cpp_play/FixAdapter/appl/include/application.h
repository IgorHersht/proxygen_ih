
#ifndef APPLICATION_H
#define APPLICATION_H

#include <boost/weak_ptr.hpp>

#include "fixI.h"

#include "session_settings_wrapper.h"
#include "logger.h"
#include "logger.h"

namespace other_fix {

	class Application : public FIX::Application, public FIX::MessageCracker {

	public:
		Application() : m_sessionManager(NULL) {}
		// Application overloads
		void onCreate(const FIX::SessionID&)  throw();
		void onLogon(const FIX::SessionID& sessionID) throw();
		void onLogout(const FIX::SessionID& sessionID) throw();
		void toAdmin(FIX::Message&, const FIX::SessionID&) throw();
		void toApp(FIX::Message&, const FIX::SessionID&) throw();
		void fromAdmin(const FIX::Message&, const FIX::SessionID&) throw();
		void fromApp(const FIX::Message& message, const FIX::SessionID& sessionID) throw();
		void onMessage(const FIX42::NewOrderSingle& message, const FIX::SessionID& sessionID) throw();
		void onMessage(const FIX42::Logon& message, const FIX::SessionID& sessionID) throw();
		void onMessage(const FIX42::OrderCancelReplaceRequest& message, const FIX::SessionID& sessionID) throw();
		void setSessionManager(SessionManager *manager) {
			m_sessionManager = manager;
		}
		void setSessionInfo(SessionsInfoPtr sessionsInfo) {
			m_sessionsInfo = sessionsInfo;
		}

		void  fixMsgFromTS(FIX::Message &message, const FIX::SessionID& sessionID) {

			try {
				timing::LatencyItems::getInstance().setTime("Fix  msg to quickfix;  fixMsgFromTS");
				FIX::Session::sendToTarget(message, sessionID);
			}
			catch (std::exception & e) {
				errorxxx(e.what());
			}
			catch (...) {
				errorxxx("Unknown exception. ");
			}
		}

	private:

		template <typename MsgT> void  fixMsgToTS(const MsgT& msg, const FIX::SessionID& sessionID);
		void fixMsgToTS(const  FIX42::Logon & msg, const FIX::SessionID& sessionID);


	private:
		boost::weak_ptr<SessionsInfo>                         m_sessionsInfo;
		SessionManager                                        *m_sessionManager;

	};

}  //namespace other_fix

#endif
