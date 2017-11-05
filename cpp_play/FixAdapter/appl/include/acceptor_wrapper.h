#ifndef _ACCEPTOR_WRAPPER_H
#define _ACCEPTOR_WRAPPER_H



#include "fix_utils.h"
#include "fixI.h"
#include <iosfwd>

namespace other_fix {


	struct AcceptorWrapper : private boost::noncopyable {
		explicit AcceptorWrapper(std::istream &fixEngineSettingsStream);

		const FIX::SessionSettings&     get_session_settings()const { return *m_settings; }
		FIX::MessageStoreFactory&       get_message_store_factory()const { return *m_storeFactory; }
		FIX::LogFactory&                get_log_factory()const { return *m_logFactory; }
		Application&                    get_aplication()const { return *m_application; }
		FixAcceptorPtr                  get_acceptor()const { return m_acceptor; }

	private:
		SessionSettingsPtr                                          m_settings;
		MessageStoreFactoryPtr                                      m_storeFactory;
		LogFactoryPtr                                               m_logFactory;
		ApplicationPtr                                              m_application;
		FixAcceptorPtr                                              m_acceptor;
	};

	typedef boost::shared_ptr<AcceptorWrapper> AcceptorWrapperPtr;

}//namespace other_fix{
#endif
