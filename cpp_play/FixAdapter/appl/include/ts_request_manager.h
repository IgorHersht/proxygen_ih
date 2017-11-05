#ifndef _REQUEST_MANAGER_H 
#define _REQUEST_MANAGER_H

#include <boost/shared_ptr.hpp>

#include "fix_utils.h"
#include "msh_field_type.h"
#include "msg_to_ts.h"
#include "fix_tsI.h"
#include "logger.h"
#include "msg_converter.h"

#include "fix/fix_42/msg_converter_fix_42.h"
#include "logger.h"

namespace other_fix {

	struct TSRequestManager : private boost::noncopyable {

		TSRequestManager(const SessionInfo &config, FixToTsRouters &sender) :
			m_config(config),
			m_sender(sender) {

		}

		template <ConverterSrcTargetID src, ConverterSrcTargetID target, typename FixMsgT>
		void  manage(const  FixMsgT &msg) {

			manage<src, target>(msg, typename FixMsgTypeMap<FixMsgT>::fix_type());
		}

		bool  allocateBuffer(TradingSystemId id, BufferPtr &buf) const {
			return m_sender.allocate(id, buf);
		}

		template <ConverterSrcTargetID src, ConverterSrcTargetID target, typename FixMsgT>
		void  convert(const  FixMsgT &msg, BufferPtr& buf) {

			MsgConverter< src, target> *converter = m_msgFactory.getMsgConverter< src, target>(m_config);
			if (!converter) {
				errorxxx("Coding error. NULL pointer converter");
				return;
			}
			converter->reset(buf);
			const std::string& seqNumStr = msg.getHeader().getField(FIX::FIELD::MsgSeqNum);
			const boost::uint_least32_t seqNum = boost::numeric_cast<boost::uint_least32_t>(boost::lexical_cast<long>(seqNumStr));
			converter->set_offset(sizeof(MsgHeader));// skip header for now
			converter->convert(msg, NullTypeObj);
			converter->insertHdr(seqNum, FixMsgTypeMap<FixMsgT>::espd_type::type, FixMsgTypeMap<FixMsgT>::espd_type::flag);
		}


		void sendEspdMsg(BufferPtr buffer, TradingSystemId ts_id) const {
			if (!buffer) {
				errorxxx("Invalid message buffer.");
				return;
			}
			const bool status = m_sender.produce(ts_id, buffer);// push to the ts fifo
			if (!status) {
				errorxxx("Cannot send buffer to TS.");
				return;
			}
		}

	private:

		template <ConverterSrcTargetID src, ConverterSrcTargetID target, typename FixMsgT, typename  FixMsgIdT>
		void  manage(const  FixMsgT &msg, FixMsgIdT idType) {
			SessionLogonInfoPtr logonInfo = m_config.getSessionLogonInfo();
			if (!logonInfo) {
				throw FIXError("Cannot find logon info.");
			}
			const TradingSystemId ts_id = logonInfo->get_tsId();
			BufferPtr buf(NULL);
			const bool status = allocateBuffer(ts_id, buf);
			if ((!buf) || (!status)) {
				errorxxx("Cannot allocate buffer for a request. ");
				return;
			}
			convert<src, target>(msg, buf);
			sendEspdMsg(buf, ts_id);
		}
		/// TODO TEMP NOT CORRECT
		template <ConverterSrcTargetID src, ConverterSrcTargetID target, typename FixMsgT>
		void  manage(const  FixMsgT &msg, FixMsgLogon) {
			// needds to be
			// user.connect(this);
			SessionLogonInfoPtr logonInfo = m_config.getSessionLogonInfo();
			if (!logonInfo) {
				throw FIXError("Cannot find logon info.");
			}
			const TradingSystemId ts_id = logonInfo->get_tsId();
			BufferPtr buf(NULL);
			const bool status = allocateBuffer(ts_id, buf);
			if ((!buf) || (!status)) {
				errorxxx("Cannot allocate buffer for a request. ");
				return;
			}
			convert<src, target>(msg, buf);
			sendEspdMsg(buf, ts_id);
		}
	private:
		const SessionInfo           &m_config;
		FixToTsRouters              &m_sender;
		MsgConverterFactory         m_msgFactory;
	};

	struct TSRequestsManager : private boost::noncopyable {
		typedef std::map< FIX::SessionID, TSRequestManagerPtr> TsRequestsT;

		TSRequestsManager(const SessionsInfo &config, FixToTsRouters &sender) :m_config(config) {
			const SessionsInfo::SessinsSettingsT& settings = config.getSessionsSettings();
			for (SessionsInfo::SessinsSettingsT::const_iterator pos = settings.begin(), end = settings.end(); pos != end; ++pos) {
				TsRequestsT::value_type value(pos->first, TSRequestManagerPtr(new TSRequestManager(*(pos->second), sender)));
				m_requests.insert(value);
			}
		}
		TSRequestManagerPtr getTSRequestManager(const FIX::SessionID& id) {
			TsRequestsT::iterator pos = m_requests.find(id);
			if (pos != m_requests.end()) {
				return pos->second;
			}
			return TSRequestManagerPtr();
		}

	private:
		const SessionsInfo        &m_config;
		TsRequestsT               m_requests;

	};

}  // namespace other_fix

#endif
