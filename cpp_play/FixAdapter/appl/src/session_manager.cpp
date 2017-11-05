
#include "fixI.h"
#include "session_manager.h"
#include "application.h"
#include "ts_request_manager.h"
#include "ts_response_manager.h"

#include "logger.h"

#include <istream>

namespace other_fix {

	SessionManager::SessionManager(std::istream &fixEngineSettingsStream) :
		m_acceptorFactory(new AcceptorWrapper(fixEngineSettingsStream)),
		m_settings(new SettingsWrapper(m_acceptorFactory->get_session_settings(), *(m_acceptorFactory->get_acceptor()))),
		m_ts_connectors(new TsConnectors(m_settings->getTradingSytemsSettings().getConnectsSettings())),
		m_fixToTsRouters(new FixToTsRouters(m_settings->getTsids())),
		m_requests(new TSRequestsManager(*(m_settings->getSessionsInfo()), *m_fixToTsRouters)),
		m_tsWriters(new TsWriters(m_settings->getTradingSytemsSettings(), *m_ts_connectors, *m_fixToTsRouters)),
		m_tsToFixRouters(new TSToFixRouters(m_settings->getTsids())),
		m_tsReaders(new  TsReaders(m_settings->getTradingSytemsSettings(), *m_ts_connectors, *m_tsToFixRouters)),
		m_tsResponseManagers(new TSResponseManagers(*m_settings, *m_tsToFixRouters)),
		m_started(false) {

		m_acceptorFactory->get_aplication().setSessionManager(this);
		m_acceptorFactory->get_aplication().setSessionInfo(m_settings->getSessionsInfo());
		m_tsResponseManagers->setSessionManager(this);
	}

	SessionManager::~SessionManager() {
		stop(true);
	}

	void SessionManager::fixMsgFromTS(FIX::Message &message, const FIX::SessionID& sessionID) {
		m_acceptorFactory->get_aplication().fixMsgFromTS(message, sessionID);
	}

	void SessionManager::wait(int seconds) {
		const boost::posix_time::time_duration sleepTime = boost::posix_time::seconds(seconds);
		while (true) {
			boost::this_thread::sleep(sleepTime);
			timing::LatencyItems::getInstance().dump(1000);
		}
	}

	void SessionManager::start() {
		if (m_started) {
			return;
		}
		try {
			m_acceptorFactory->get_acceptor()->start();
			m_tsWriters->start();
			m_tsReaders->start();
			m_tsResponseManagers->start();
			m_started = true;
		}
		catch (std::exception & e) {
			std::string msg("Cannot start SessionManager ");
			msg += e.what();
			throw FIXError(msg);
		}
		catch (...) {
			std::string msg("Cannot start SessionManager. Unknown exception.  ");
			throw FIXError(msg);
		}
	}
	void SessionManager::stop(bool force) {
		if (!m_started) {
			return;
		}
		try {
			m_acceptorFactory->get_acceptor()->stop(force);
			m_tsWriters->stop();
			m_tsReaders->stop();
			m_tsResponseManagers->stop();
			m_started = false;
		}
		catch (std::exception & e) {
			std::string msg("Cannot stop SessionManager ");
			msg += e.what();
			errorxxx(msg);
		}
		catch (...) {
			std::string msg("Cannot stop SessionManager. Unknown exception.  ");
			errorxxx(msg);
		}
	}

}
