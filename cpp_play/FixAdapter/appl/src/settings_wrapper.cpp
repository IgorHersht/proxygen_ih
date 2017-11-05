
#include "settings_wrapper.h"
#include "session_settings_wrapper.h"
#include "acceptor_wrapper.h"
#include "ts_settings.h"



namespace other_fix {

	SettingsWrapper::SettingsWrapper(const FIX::SessionSettings& settings, FIX::ThreadedSocketAcceptor& acceptor) :
		m_sessionsInfo(new SessionsInfo(settings, acceptor)),
		m_tsInfo(new TradingSytemsSettings(settings, *m_sessionsInfo)) {

	}

	const TradingSystemsId& SettingsWrapper::getTsids() const { return m_tsInfo->getTsids(); }



}//namespace other_fix{

