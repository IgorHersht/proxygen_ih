#ifndef _RESPONSE_MANAGER_H
#define _RESPONSE_MANAGER_H

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include "settings_wrapper.h"
#include "msg_from_ts.h"
#include "fix_tsI.h"
#include "msg_converter.h"
#include "logger.h"

namespace other_fix {
	class SessionManager;

	struct TSResponseManager : private boost::noncopyable {

		typedef ThreadWrapper <TSResponseManager, TSResponseManager*>  ThreadEntryT;
		typedef boost::shared_ptr<ThreadEntryT> ThreadEntryPtr;
		typedef TSToFixRouter RouterT;

		TSResponseManager(const SessionsInfo &config, RouterT &router) :
			m_config(config), m_router(router), m_converterSrcId(UNKNOWN_CONVERTER_ST_ID) {
			init(config);
		}
		void operator()() {
			try {
				boost::asio::detail::signal_blocker blocker;
				threadEntryWrapper(*this, &TSResponseManager::manageEspdMsg);
			}
			catch (std::exception &e) {
				errorxxx(e.what());
			}
			catch (...) {
				errorxxx("Unknown exception. ");
			}
		}
		void setSessionManager(SessionManager *sessionManager) {
			m_sessionManager = sessionManager;
		}
		void start() {//star thread
			const bool status = m_thread->start();
			if (!status) {
				errorxxx("Cannot stop a response thread");
			}
		}
		void stop() {//stop theard TODO
			warningxxx("TSResponseManager:: stop not implemeted ");
			/*
			 const bool status = m_thread->stop() ;
			 if(!status) {
				warningxxx("Cannot stop a response thread");
			 }
			*/
		}
		void exitThread() {  //TODO ???

		}

	private:

		typedef MsgConverter <  UST_ID_ST, FIX_4_2_ST  >::VBaseT ConverterBaseT; // the same for any ts-fix combination
		ConverterBaseT* getConverterBase() {
			return m_msgFactory.getMsgConverter< UST_ID_ST, FIX_4_2_ST >(m_config);
		}
		void init(const SessionsInfo &config);
		void manageEspdMsg();

		void manageEspdMsg(BufferPtr buff, FixMessageInfo &fixMsg) {// target-src cpecific
			ConverterBaseT *converterBase = getConverterBase();
			if (!converterBase) {
				errorxxx("NULL pointer message converter.");
				return;
			}
			converterBase->reset(buff);
			converterBase->parseBuffer();
			converterBase->convert(NullTypeObj, fixMsg);

			if (!fixMsg.first) {
				return;
			}
			if (!fixMsg.second || (*fixMsg.second == fix_consts::INVALID_SESSION_ID)) {
				const  boost::uint_least16_t msgType = converterBase->get_msgType();
				std::string err("An error parsing Fix session id from  message type "); err += boost::lexical_cast<std::string>(msgType);
				err += ". The message is ignored";
				warningxxx(err);
				return;
			}

			const std::string& protocol = fixMsg.second->getBeginString();

			switch (m_converterSrcId)
			{
			case UST_ID_ST: {
				if (protocol == FIX::BeginString_FIX42) {
					convert<UST_ID_ST, FIX_4_2_ST>(buff, fixMsg);
				}
				else if (protocol == FIX::BeginString_FIX44) {
					convert<UST_ID_ST, FIX_4_4_ST>(buff, fixMsg);
				}
				else {
					std::string err("Invalid Fix protocal "); err += protocol;
					err += " .";
					warningxxx(err);
				}
				break;
			}
			case ELX_ID_ST: {
				if (protocol == FIX::BeginString_FIX42) {
					convert<ELX_ID_ST, FIX_4_2_ST>(buff, fixMsg);
				}
				else if (protocol == FIX::BeginString_FIX44) {
					convert<ELX_ID_ST, FIX_4_4_ST>(buff, fixMsg);
				}
				else {
					std::string err("Invalid Fix protocal "); err += protocol;
					err += " .";
					warningxxx(err);
				}
				break;
			}
			default: {
				std::string err("Invalid converter source id "); err += boost::lexical_cast<std::string>(m_converterSrcId);
				err += " .";
				warningxxx(err);
				break;
			}
			}
		}

		template < ConverterSrcTargetID src, ConverterSrcTargetID target>
		void convert(BufferPtr buff, FixMessageInfo &fixMsg) {

			MsgConverter <  src, target  >* converter = m_msgFactory.getMsgConverter< src, target  >(m_config);// never NULL;
			converter->reset(buff);
			converter->convertSrc(NullTypeObj, fixMsg);
			converter->convertTarget(NullTypeObj, fixMsg);
		}

	private:
		const SessionsInfo          &m_config;
		MsgConverterFactory         m_msgFactory;
		RouterT                     &m_router;
		ThreadEntryPtr              m_thread;
		ConverterSrcTargetID        m_converterSrcId;
		SessionManager              *m_sessionManager;
	};

	struct TSResponseManagers : private boost::noncopyable {

		typedef std::map< TradingSystemId, TSResponseManagerPtr >  TSResponseManagersT;
		typedef TSResponseManager::RouterT RouterT;
		typedef TSToFixRouters RoutersT;

		TSResponseManagers(const SettingsWrapper &settings, RoutersT &routers) {
			init(settings, routers);
		}

		~TSResponseManagers() {
			stop();
		}

		void setSessionManager(SessionManager *sessionManager) {
			for (TSResponseManagersT::iterator pos = m_managers.begin(), end = m_managers.end(); pos != end; ++pos) {
				(pos->second)->setSessionManager(sessionManager);
			}
		}

		void start() {
			for (TSResponseManagersT::iterator pos = m_managers.begin(), end = m_managers.end(); pos != end; ++pos) {
				(pos->second)->start();
			}
		}

		void stop() {
			for (TSResponseManagersT::iterator pos = m_managers.begin(), end = m_managers.end(); pos != end; ++pos) {
				(pos->second)->stop();
			}
		}

	private:
		void init(const SettingsWrapper &settings, RoutersT &routers);
	private:
		TSResponseManagersT                                       m_managers;
	};

} // namespace other_fix

#endif
