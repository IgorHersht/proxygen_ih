#ifndef _SESSION_MANAGER_H
#define _SESSION_MANAGER_H


#include <iosfwd>

#include "fix_utils.h"
#include "session_settings_wrapper.h"
#include "acceptor_wrapper.h"
#include "ts_request_manager.h"
#include "ts_response_manager.h"
#include "ts_writer.h"
#include "ts_reader.h"
#include "settings_wrapper.h"
#include "fix_tsI.h"
#include "msg_converter_I.h"



namespace other_fix {


	struct  SessionManager : private boost::noncopyable {

		explicit SessionManager(std::istream &fixEngineSettingsStream);//main thread only (no sync)
		~SessionManager(); //main thread only (no sync)
		void start();//main thread only (no sync)
		void stop(bool force = false); //main thread only (no sync)
		void wait(int seconds = 5); //main thread only (no sync)

		template < ConverterSrcTargetID src, ConverterSrcTargetID target, typename SrcSubT, typename TargetSubT>
		void manage(const SrcSubT &sSrc, TargetSubT &sTarget, const typename MsgConverterBaseWr<src, target>::session_id_type  &sessionID) {
			manageImpl<src, target>((typename MsgConverterBaseWr<src, target>::manager_type*)NULL, sSrc, sTarget, sessionID);
		}

		void fixMsgFromTS(FIX::Message &message, const FIX::SessionID& sessionID);
	private:

		template <ConverterSrcTargetID src, ConverterSrcTargetID target, typename FixMsgT>
		void manageImpl(TSRequestsManager*, const  FixMsgT & msg, NullType, const FIX::SessionID& sessionID) {
			TSRequestManagerPtr rm = m_requests->getTSRequestManager(sessionID);
			if (!rm) {
				std::string err = "Cannot manage fix message. Invalid session id "; err += sessionID.toString(); err += " .";
				throw FIXError(err);
			}
			rm->manage<src, target>(msg);
		}

	private:
		AcceptorWrapperPtr                      m_acceptorFactory;
		SettingsWrapperPtr                      m_settings;
		TsConnectorsPtr                         m_ts_connectors;
		FixToTsRouters::FixTsRoutersPtr         m_fixToTsRouters;
		TSRequestsManagerPtr                    m_requests;
		TsWritersPtr                            m_tsWriters;
		TSToFixRouters::FixTsRoutersPtr         m_tsToFixRouters;
		TsReadersPtr                            m_tsReaders;
		TSResponseManagersPtr                  m_tsResponseManagers;
		bool                                   m_started;
	};





}//namespace other_fix{
#endif
