#ifndef MSG_TO_TS_H
#define MSG_TO_TS_H
#include <boost/call_traits.hpp>

#include "fixI.h"
#include "ext_property.h"
#include "buffer_cache.h"
#include "session_settings_wrapper.h"



namespace other_fix {

	struct MsgToTs : public EspeedMsgEncoder {
		typedef  SessionInfo  ArgT;
		typedef EspeedMsgEncoder Base;
		explicit MsgToTs(const SessionInfo& config) :m_config(config) {
		}

		template <typename SrcSubT, typename TargetSubT> void convert(const SrcSubT &sSrc, TargetSubT&) {
			appendMsg(sSrc);
		}

		template <typename FixMsgT> void appendMsg(const  FixMsgT & msg) {

			try {
				appendMgsBody(msg, typename FixMsgTypeMap<FixMsgT>::fix_type());
			}
			catch (std::exception & e) {
				throw;
			}
			catch (...) {
				throw;
			}
		}

	protected:

		template <typename FixMsgT>
		void appendMgsBody(const  FixMsgT & msg, FixMsgLogon)
		{
			try {
				SessionLogonInfoPtr info = m_config.getSessionLogonInfo();
				if (!info) {
					throw FIXError(std::string("Invalid Logon info "));
				}

				appendField<CFETP_FIELD_SESSIONID>(info->get_sessId());
				appendField<CFETP_FIELD_TS_ID>(info->get_tsId());
				appendField<CFETP_FIELD_USERNAME>(info->get_userName());
				appendField<CFETP_FIELD_TS_PREF>(info->get_tsPref());
				appendField <CFETP_FIELD_PASSWORD>(info->get_passWord());
				const long gmtDelta = getGmtDelta();
				appendField<CFETP_FIELD_GMT_DELTA>(gmtDelta);
				appendFieldByKey<CFETP_FIELD_IDENT_COMPANY>("IDENT_COMPANY");
				appendFieldByKey<CFETP_FIELD_IDENT_APPLICATION>("IDENT_APPLICATION");
				appendFieldByKey<CFETP_FIELD_IDENT_VERSION>("IDENT_VERSION");
				appendFieldByKey <CFETP_FIELD_COMPONENT_TYPE>("componentType");
				appendFieldByKey <CFETP_FIELD_IDENT_USER_OS>("IDENT_USER_OS");
				appendFieldByKey <CFETP_FIELD_IDENT_SDK_VERSION_ID>("sdk_version_id");
				appendFieldByKey <CFETP_FIELD_GMT_DELTA>("gmt_delta");// dup need second time?
				appendField<CFETP_FIELD_GMT_DELTA>(gmtDelta);// dup need second time?       
			}
			catch (std::exception & e) {
				throw;
			}
			catch (...) {
				throw;
			}
		}

		template <typename FixMsgT>
		void appendMgsBody(const  FixMsgT & msg, FixMsgNewOrderSingle) {
			appendOrderBody<>(msg);
		}

		template <typename FixMsgT>
		void appendMgsBody(const  FixMsgT & msg, FixMsgOrderCancelReplaceRequest) {
			appendOrderBody<>(msg);
			FIX::OrigClOrdID origClOrdID;  msg.get(origClOrdID);
			appendField <CFETP_FIELD_ORIG_CLIENT_ORDER_ID>(origClOrdID);
		}

		template<typename OrderT> void appendOrderBody(const OrderT & msg) {
			try {
				SessionLogonInfoPtr info = m_config.getSessionLogonInfo();
				if (!info) {
					throw FIXError(std::string("Invalid Logon info "));
				}
				//appendObj(125, 4 );    // should be compile error
				appendObj(buff_cache::SUBMIT_ORDER, buff_cache::SubmitOrder::FLD_NUM);
				if (m_cache) {
					appendBuffer(m_cache->get_buf(), m_cache->get_size(), m_cache->get_fields_num());
				}
				else {
					char * start = get_pos();
					const boost::uint_least16_t startNumFields = m_numFields;
					appendField<CFETP_FIELD_SESSIONID>(info->get_sessId());
					appendField<CFETP_FIELD_USERNAME>(info->get_userName());
					char * end = get_pos();
					const boost::uint_least16_t extraFld = m_numFields - startNumFields;
					m_cache = buff_cache::SessionBufferPtr(new buff_cache::SessionBuffer(start, end, extraFld));
				}
				try {
					FIX::SenderSubID senderSubID;  msg.getHeader().getField(senderSubID);
					appendField <CFETP_FIELD_SOURCE_ID>(senderSubID);
				}
				catch (FIX::FieldNotFound &fld) {
				}
				FIX::Symbol symbol;  msg.get(symbol);
				appendField <CFETP_FIELD_INSTNAME>(symbol);

				const TradingSystemId ts_id = info->get_tsId();
				appendField<CFETP_FIELD_TS_ID>(ts_id);
				// needds to be
				// user.get_xsessId(); 
				appendField<CFETP_FIELD_EXT_TS_SESSIONID>(info->get_xsessId(ts_id));

				FIX::ClOrdID clOrdID;  msg.get(clOrdID);
				appendField <CFETP_FIELD_CLIENT_ORDER_ID>(clOrdID);

				FIX::OrderQty orderQty; msg.get(orderQty);
				appendField <CFETP_FIELD_SIZE>(orderQty);

				FIX::Side side; msg.get(side);
				appendField <CFETP_FIELD_INDICATOR>(ESPD_FIX_MAP.getIndicator(side));

				FIX::OrdType ordType; msg.get(ordType);
				FIX::Price price;
				if ((ordType == FIX::OrdType_MARKET) && msg.isSet(price)) {
					throw RejectError("Market order cannot have price. ");
				}
				msg.get(price);
				appendField <CFETP_FIELD_PRICE>(price);
			}
			catch (...) {
				throw;
			}
		}


	private:
		template < boost::uint_least16_t id> void appendFieldByKey(const std::string &key) {
			typedef typename IdType< id>::type::native_type native_type;
			native_type data = m_config.getSettingsValue< native_type >(key);
			appendField<id>(data);
		}


	private:
		const SessionInfo&                      m_config;
		buff_cache::SessionBufferPtr            m_cache;

	};



	typedef boost::shared_ptr<MsgToTs> MsgToTsPtr;


}// namespace other_fix
#endif
