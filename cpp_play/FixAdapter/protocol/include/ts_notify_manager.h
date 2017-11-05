#ifndef _NOTIFY_MANAGER_H
#define _NOTIFY_MANAGER_H

#include <boost/thread.hpp>

#include "fixI.h"
#include "fix_espd_maper.h"
#include "msg_buffer_map.h"
#include "logger.h"
#include "fix_consts.h"
#include "unique_generator.h"
#include "session_settings_wrapper.h"
#include "msh_field_type.h"


namespace other_fix {

	namespace notifyNS {

		enum Cmd {
			ORDER_QUEUED = 0, ORDER_REJECTED, ORDER_EXECUTED, ORDER_CANCELLED, TRADE_CONFIRM,
			CANCEL_ORDER_ACCEPTED, CANCEL_ORDER_REJECTED,
			CMD_NUM
		};


		template< boost::uint_least32_t cmd> struct CmdMap;

		template<> struct CmdMap< CFETI_ORDER_QUEUED > { enum { espdCmd = CFETI_ORDER_QUEUED };      enum { notifyCmd = ORDER_QUEUED }; };
		template<> struct CmdMap< CFETI_ORDER_REJECTED > { enum { espdCmd = CFETI_ORDER_REJECTED };    enum { notifyCmd = ORDER_REJECTED }; };
		template<> struct CmdMap< CFETI_ORDER_EXECUTED> { enum { espdCmd = CFETI_ORDER_EXECUTED };    enum { notifyCmd = ORDER_EXECUTED }; };
		template<> struct CmdMap< CFETI_ORDER_CANCELLED > { enum { espdCmd = CFETI_ORDER_CANCELLED };    enum { notifyCmd = ORDER_CANCELLED }; };
		template<> struct CmdMap< CFETI_TRADE_CONFIRM > { enum { espdCmd = CFETI_TRADE_CONFIRM };     enum { notifyCmd = TRADE_CONFIRM }; };
		template<> struct CmdMap< CFETI_CANCEL_ORDER_ACCEPTED> { enum { espdCmd = CFETI_CANCEL_ORDER_ACCEPTED }; enum { notifyCmd = CANCEL_ORDER_ACCEPTED }; };
		template<> struct CmdMap< CFETI_CANCEL_ORDER_REJECTED> { enum { espdCmd = CFETI_CANCEL_ORDER_REJECTED }; enum { notifyCmd = CANCEL_ORDER_REJECTED }; };

	}

	struct TSNotifyManager : private boost::noncopyable {

		explicit TSNotifyManager(const SessionsInfo &config, const FieldActionMap::FieldPosT &fields) :
			m_config(config) {
			init(fields);
		}

		void setExecId(const std::string &id) {
			m_msg.getMsg().setField(FIX::ExecID(id));
		}

		void manageEspdMsg(boost::uint_least32_t cmd, FixMessageInfo &fixMsg) {
			using namespace notifyNS;
			switch (cmd)
			{
			case CFETI_ORDER_QUEUED: { return manageEspdMsg <notifyNS::ORDER_QUEUED >(fixMsg); }
			case CFETI_ORDER_REJECTED: { return manageEspdMsg <notifyNS::ORDER_REJECTED >(fixMsg); }
									   //case CFETI_ORDER_EXECUTED:            return  manageEspdMsg(EspdCmdT< CFETI_ORDER_EXECUTED >(), fix_msg );
									   //case CFETI_TRADE_CONFIRM:             return  manageEspdMsg(EspdCmdT< CFETI_TRADE_CONFIRM >(),  fix_msg );
									   //case CFETI_CANCEL_ORDER_ACCEPTED:     return  manageEspdMsg(EspdCmdT< CFETI_CANCEL_ORDER_ACCEPTED >(), fix_msg );
									   //case CFETI_CANCEL_ORDER_REJECTED:     return  manageEspdMsg(EspdCmdT< CFETI_CANCEL_ORDER_REJECTED >(),fix_msg );
			};
			std::string err = "Notify message has unknown cmd "; err += boost::lexical_cast<std::string>(cmd);
			err += "The message is ignored. ";
			warningxxx(err);
		}

	private:

		typedef boost::array<FieldActionMapPtr, notifyNS::CMD_NUM> FieldActionMapsT;
		template< notifyNS::Cmd cmd> struct CmdT { enum { value = cmd }; };
		typedef boost::array<FIX::Message, notifyNS::CMD_NUM> InitFixMsgs;

		void init(const FieldActionMap::FieldPosT &fields) {
			for (FieldActionMapsT::iterator pos = m_actionMaps.begin(), end = m_actionMaps.end(); pos != end; ++pos) {
				*pos = FieldActionMapPtr(new FieldActionMap(fields));
			}
			// init all cmd
			init(CmdT< notifyNS::ORDER_QUEUED >());
			init(CmdT< notifyNS::ORDER_REJECTED >());
			//...
		}
		void init(CmdT< notifyNS::ORDER_QUEUED >) {

			FIX::Message &msg = m_initMsgs[notifyNS::ORDER_QUEUED];
			FIX::Header &header = msg.getHeader();
			header.setField(FIX::MsgType(FIX::MsgType_ExecutionReport));
			msg.setField(FIX::ExecType(FIX::ExecType_NEW));
			msg.setField(FIX::ExecTransType(FIX::ExecTransType_NEW));
			msg.setField(FIX::OrdStatus(FIX::OrdStatus_NEW));
			msg.setField(FIX::LastShares(0));
			msg.setField(FIX::CumQty(0));
			msg.setField(FIX::AvgPx(0));
			msg.setField(FIX::LastPx(0));
		}
		void init(CmdT< notifyNS::ORDER_REJECTED >) {
		}

		template <typename notifyNS::Cmd cmd> void manageEspdMsg(FixMessageInfo &fixMsg) {

			m_msg.setMsg(m_initMsgs[cmd]);
			manageActions <CmdT< cmd> >();
			FIX::Message& executionReport = m_msg.getMsg();
			const FIX::SessionID& id = m_msg.get_sessionID(m_config);
			manageEspdMsg(executionReport, id, CmdT<cmd>());
			fixMsg.first = &executionReport;
			fixMsg.second = &id;
		}

		template <typename CmT> void manageActions() {

			FieldActionMapPtr aMap = m_actionMaps[CmT::value];
			if (!aMap) {
				throw ESPDError("Codding error in TSNotifyManager");
			}

			for (FieldActionMap::FieldActIterT pos = aMap->begin(); !aMap->isEnd(pos); pos = aMap->next(pos)) {
				FixAction act = aMap->getAction(pos);
				if (!act) {
					const boost::uint_least16_t id = aMap->getId(pos);
					act = getFixAction(id, CmT());
					aMap->setAction(pos, act);
				}
				act(aMap->getPos(pos), &m_msg);
			}

		}

		FixAction getFixAction(boost::uint_least16_t id, CmdT< notifyNS::ORDER_QUEUED > cmd)const {
			switch (id) {
			case CFETP_FIELD_CLIENT_ORDER_ID:           return Act<CFETP_FIELD_CLIENT_ORDER_ID, FixMsgExecutionReport>::make;
			case CFETP_FIELD_EXT_TS_SESSIONID:          return Act<CFETP_FIELD_EXT_TS_SESSIONID, FixMsgExecutionReport>::make;
			case CFETP_FIELD_TS_ID:                     return Act<CFETP_FIELD_TS_ID, FixMsgExecutionReport>::make;
			case CFETP_FIELD_ORIG_CLIENT_ORDER_ID:      return Act<CFETP_FIELD_ORIG_CLIENT_ORDER_ID, FixMsgExecutionReport>::make;
			case CFETP_FIELD_INSTNAME:                  return Act<CFETP_FIELD_INSTNAME, FixMsgExecutionReport>::make;
			case CFETP_FIELD_PRICE:                     return Act<CFETP_FIELD_PRICE, FixMsgExecutionReport>::make;
			case CFETP_FIELD_SIZE:                      return Act<CFETP_FIELD_SIZE, FixMsgExecutionReport>::make;
			case CFETP_FIELD_INDICATOR:                 return Act<CFETP_FIELD_INDICATOR, FixMsgExecutionReport>::make;
			case CFETP_FIELD_ID:                        return Act<CFETP_FIELD_ID, FixMsgExecutionReport>::make;
			};
			return FieldActionMap::dummyAction;
		}

		FixAction getFixAction(boost::uint_least16_t id, CmdT< notifyNS::ORDER_REJECTED > cmd)const {
			warningxxx("ORDER_REJECTED message has not been implemented yet");
			return FieldActionMap::dummyAction;
		}

		void manageEspdMsg(FIX::Message& msg, const FIX::SessionID& sessionID, CmdT< notifyNS::ORDER_QUEUED >) {
			FIX::OrderQty orderQty; msg.getField(orderQty);
			msg.setField(FIX::LeavesQty(orderQty));
		}
		void manageEspdMsg(FIX::Message& msg, const FIX::SessionID& sessionID, CmdT< notifyNS::ORDER_REJECTED  >) {

		}

	private:
		const SessionsInfo                  &m_config;
		FieldActionMapsT                    m_actionMaps;
		FixMsgWr                            m_msg;
		InitFixMsgs                         m_initMsgs;
	};


}// namespace other_fix{
#endif
