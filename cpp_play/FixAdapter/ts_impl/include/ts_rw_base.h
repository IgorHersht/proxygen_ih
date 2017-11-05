#ifndef _TS_RW_BASE_H
#define _TS_RW_BASE_H

#include "ts_connector.h"
#include "ts_settings.h"
#include "thread_wrapper.h"
#include "fix_utils.h"
#include "fix_tsI.h"
#include "logger.h"

namespace other_fix {


	class TsConnectors : private boost::noncopyable {
		typedef TradingSytemsSettings::ConnectsSettingsT SettingsT;
		typedef std::map<TradingSystemId, TsConnectorPtr>  ConnectorsT;
	public:
		explicit TsConnectors(const SettingsT &setings) {
			for (SettingsT::const_iterator pos = setings.begin(), end = setings.end(); pos != end; ++pos) {
				ConnectorsT::value_type value(pos->first, TsConnectorPtr(new TsConnector(*(pos->second))));
				m_connectors.insert(value);
			}
		}

		TsConnectorPtr find(TradingSystemId id) const {
			ConnectorsT::const_iterator pos = m_connectors.find(id);
			if (pos != m_connectors.end()) {
				return pos->second;
			}
			return TsConnectorPtr();
		}
	private:
		ConnectorsT              m_connectors;
	};


	template <typename ImplT> struct TsRW : public ImplT, private boost::noncopyable {

		typedef ThreadWrapper <TsRW, TsRW*>  ThreadEntryT;
		typedef boost::shared_ptr<ThreadEntryT> ThreadEntryPtr;
		typedef typename ImplT::RouterT RouterT;

		TsRW(const TradingSytemSettings  &settings, TsConnector  &connector, RouterT &router) :
			ImplT(settings, connector, router) {
			m_thread = ThreadEntryPtr(new ThreadEntryT(this));
		}
		~TsRW() {
			stop();
		}

		void operator()() {
			try {
				boost::asio::detail::signal_blocker blocker;
				this->m_connector.connect();
				threadEntryWrapper((ImplT&)(*this), &ImplT::threadFunction);
			}
			catch (std::exception &e) {
				std::string msg(e.what()); msg += " Exiting ";   msg += ImplT::IMPL_TYPE; msg += ".";
				errorxxx(msg);
			}
			catch (...) {
				std::string msg("Unknown exception. "); msg += " Exiting ";   msg += ImplT::IMPL_TYPE; msg += ".";
				errorxxx(msg);
			}
		}// thread entry
		void start() {//star thread
		   // not thread safe
		   // std::ostringstream os;
		 //   os << this->m_settings.to_string() << ", connection settings ("
		 //      << this->m_connector.to_string() << ")";
		 //   logxxx(os.str());
			const bool status = m_thread->start();
			if (!status) {
				logxxx("Cannot stop a response thread");
			}
		}
		void stop() {//stop theard, TODO
			warningxxx("TsRW:: stop not implemeted ");
			/*
		   const bool status = m_thread->stop() ;
		   if(!status) {
			  warningxxx("Cannot stop a response thread");
		   }
		   */
			this->m_connector.disconnect();// Not correct Not thread safe.
		}
		void exitThread() {

		}

	private:
		ThreadEntryPtr                           m_thread;
	};

	template<typename ImplT > struct TsRsWs {

		typedef boost::shared_ptr< TsRW< ImplT > > TsRWPtr;
		typedef std::map< TradingSystemId, TsRWPtr >  TsRWs;
		typedef typename ImplT::RouterT RouterT;
		typedef FixTsRouters<RouterT> RoutersT;

		TsRsWs(const TradingSytemsSettings &settings, TsConnectors &connectors, RoutersT &routers) {
			init(settings, connectors, routers);
		}

		~TsRsWs() {
			stop();
		}

		void start() {
			for (typename TsRWs::iterator pos = m_rws.begin(), end = m_rws.end(); pos != end; ++pos) {
				(pos->second)->start();
			}
		}

		void stop() {
			for (typename TsRWs::iterator pos = m_rws.begin(), end = m_rws.end(); pos != end; ++pos) {
				(pos->second)->stop();
			}
		}

	private:

		void init(const TradingSytemsSettings &settings, TsConnectors &connectors, RoutersT &routers) {
			typedef TradingSytemsSettings::TsSettingsT SMapT;
			try {
				const SMapT &ss = settings.getTsSettings();
				for (SMapT::const_iterator pos = ss.begin(), end = ss.end(); pos != end; ++pos) {
					if (!(pos->second)) {
						warningxxx("Cannot find Ts.");
						continue;
					}
					TsConnectorPtr con = connectors.find(pos->first);
					if (!con) {
						warningxxx("Cannot find connection.");
						continue;
					}
					typename RouterT::FixTsRouterPtr router = routers.find(pos->first);
					if (!router) {
						warningxxx("Cannot find router.");
						continue;
					}

					TsRWPtr ptr(new TsRW< ImplT >(*(pos->second), *con, *router));
					m_rws.insert(typename TsRWs::value_type(pos->first, ptr));
				}
			}
			catch (...) {
				errorxxx("cannot init TsWriters");
			}
		}

	private:
		TsRWs                                       m_rws;
	};

}  //namespace other_fix{

#endif
