
#include "acceptor_wrapper.h"
#include "application.h"


namespace other_fix {

	AcceptorWrapper::AcceptorWrapper(std::istream &fixEngineSettingsStream) :
		m_settings(new FIX::SessionSettings(fixEngineSettingsStream)),
		m_storeFactory(new FIX::FileStoreFactory(*m_settings)),
		m_logFactory(new FIX::FileLogFactory(*m_settings)),
		m_application(new Application),
		m_acceptor(new FIX::ThreadedSocketAcceptor(*m_application, *m_storeFactory, *m_settings, *m_logFactory)) {
	}


}//namespace other_fix{

