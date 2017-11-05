#include "application.h"
#include "session_manager.h"
#include "logger.h"

namespace other_fix {

	// called from maim thread
	void Application::onCreate(const FIX::SessionID& sessionID) throw() {

	}
	void Application::onLogon(const FIX::SessionID& sessionID) throw() {

	}
	void Application::onLogout(const FIX::SessionID& sessionID) throw() {

	}
	void Application::toAdmin(FIX::Message& message, const FIX::SessionID& sessionID) throw() {
		// called just before sending admin message to client
	}
	void Application::toApp(FIX::Message& message, const FIX::SessionID& sessionID)throw() {
		// called just before sending app message to client
	}
	// called from FIX::SocketAcceptor thread
	void Application::fromAdmin(const FIX::Message& message, const FIX::SessionID& sessionID)throw() {
		try {
			crack(message, sessionID);
		}
		catch (std::exception & e) {
			errorxxx(e.what());
		}
		catch (...) {
			errorxxx("Unknown exception. ");
		}
	}

	// called from FIX::SocketAcceptor thread
	void Application::fromApp(const FIX::Message& message, const FIX::SessionID& sessionID) throw() {
		//boost::asio::detail::signal_blocker blocker; // TODO move to the thread start in quickfix
		try {
			crack(message, sessionID);
		}
		catch (std::exception & e) {
			errorxxx(e.what());
		}
		catch (...) {
			errorxxx("Unknown exception. ");
		}
	}

	template void Application::fixMsgToTS(const FIX42::NewOrderSingle& msg, const FIX::SessionID& sessionID);
	template void Application::fixMsgToTS(const FIX42::OrderCancelReplaceRequest& msg, const FIX::SessionID& sessionID);


	template <typename FixMsgT> void Application::fixMsgToTS(const  FixMsgT & msg, const FIX::SessionID& sessionID) {
		if (!m_sessionManager) {
			throw FIXError(" Session Manager has not been set. ");
		}
		try {
			timing::LatencyItems::getInstance().setTime("Received Fix Mgs from quickfix. Application::fixMsgToTS");
			const ConverterSrcTargetID target = UST_ID_ST;
			switch (target) {
			case UST_ID_ST: {
				m_sessionManager->manage< (ConverterSrcTargetID)FixProtocolMap< FixMsgT >::PROTOCOL, UST_ID_ST >(msg, NullTypeObj, sessionID);
				break;
			}
			case ELX_ID_ST: {
				m_sessionManager->manage< (ConverterSrcTargetID)FixProtocolMap< FixMsgT >::PROTOCOL, ELX_ID_ST >(msg, NullTypeObj, sessionID);
				break;
			}
			default: RejectError("Unknown source-target converter id.");
			};
		}
		catch (RejectError & e) {
			errorxxx(std::string(e.what()) + " Reject message has not been implemented yet. ");
		}
		catch (std::exception & e) {
			errorxxx(e.what());
		}
		catch (...) {
			errorxxx("Unknown exception. ");
		}
	}

	// MessageCracker overloads
	// called from FIX::SocketAcceptor thread
	void Application::onMessage(const FIX42::NewOrderSingle& message, const FIX::SessionID& sessionID) throw() {
		fixMsgToTS(message, sessionID);
	}
	void Application::onMessage(const FIX42::OrderCancelReplaceRequest & message, const FIX::SessionID& sessionID) throw() {
		fixMsgToTS(message, sessionID);
	}
	// called from FIX::SocketAcceptor thread
	void Application::onMessage(const FIX42::Logon& message, const FIX::SessionID& sessionID) throw() {
		if (!m_sessionManager) {
			errorxxx("Code error. m_sessionManager is a NULL pointer ");
			return;
		}
		try {
			m_sessionManager->manage< (ConverterSrcTargetID)FixProtocolMap< FIX42::Logon >::PROTOCOL, ANY_TS_ST >(message, NullTypeObj, sessionID);
		}
		catch (RejectError & e) {
			errorxxx(std::string(e.what()) + " Reject message has not been implemented yet. ");
		}
		catch (std::exception & e) {
			errorxxx(e.what());
		}
		catch (...) {
			errorxxx("Unknown exception. ");
		}
	}


}//namespace other_fix{
