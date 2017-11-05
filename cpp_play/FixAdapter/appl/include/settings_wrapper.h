#ifndef _SETTINGS_WRAPPER_H
#define _SETTINGS_WRAPPER_H

#include "fix_utils.h"
#include "ts_settings.h"


namespace other_fix {

	struct SettingsWrapper : private boost::noncopyable {
		SettingsWrapper(const FIX::SessionSettings&, FIX::ThreadedSocketAcceptor&);

		SessionsInfoPtr getSessionsInfo() const { return m_sessionsInfo; }
		const TradingSytemsSettings& getTradingSytemsSettings() const { return *m_tsInfo; }
		const TradingSystemsId& getTsids() const;
	private:
		SessionsInfoPtr                         m_sessionsInfo;
		TradingSytemsSettingsPtr                m_tsInfo;
	};


}//namespace other_fix{
#endif
