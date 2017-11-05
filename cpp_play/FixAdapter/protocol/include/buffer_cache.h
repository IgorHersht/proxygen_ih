#ifndef BUFFET_CACHE_H
#define BUFFET_CACHE_H

#include <vector>
#include "msh_field_type.h"
#include "endian_wrapper.h"
#include "espdI.h"
#include "logger.h"
#include "fix_utils.h"

namespace other_fix {
	namespace buff_cache {

		struct  Order {
			typedef boost::tuple<
				MsgField< MsgFieldT< CFETI_FIELDTYPE_BYTE > >, // msgType
				MsgField< MsgFieldT< CFETI_FIELDTYPE_UINT32 > >, // requestId
				MsgField< MsgFieldT< CFETI_FIELDTYPE_STRING> >,  // status
				MsgField< MsgFieldT< CFETI_FIELDTYPE_UINT32> > //oderInfo
			> FieldsT;
			enum { FLD_NUM = boost::tuples::length<FieldsT>::value };

			Order() :
				m_fields(
					MsgField< MsgFieldT< CFETI_FIELDTYPE_BYTE > >(CFETP_FIELD_MSGTYPE, CFETP_ORDER),
					MsgField< MsgFieldT< CFETI_FIELDTYPE_UINT32 > >(CFETP_FIELD_REQUEST_ID, 0),
					MsgField< MsgFieldT< CFETI_FIELDTYPE_STRING> >(CFETP_FIELD_STATUSTEXT),
					MsgField< MsgFieldT< CFETI_FIELDTYPE_UINT32> >(CFETP_FIELD_ORDER_INFO_TYPE, CFETI_ORDERINFO_EXTENDED_PROPERTIES)
				)
			{}

		private:
			FieldsT  m_fields;


		};
		//const Order ORDER;

		struct SubmitOrder {
			SubmitOrder() : m_cmd(CFETP_FIELD_CMD, CFETC_SUBMIT_ORDER) {}
			enum { FLD_NUM = Order::FLD_NUM + 1 };
		private:
			Order     											m_order;
			MsgField< MsgFieldT< CFETI_FIELDTYPE_UINT32 > > 	m_cmd;
		};


		const SubmitOrder SUBMIT_ORDER;



		struct SessionBuffer {
			SessionBuffer() :m_numFields(0) {}
			// client should test if start and end are OK
			SessionBuffer(const char *start, const char *end, boost::uint_least16_t	numFields) :
				m_buffer(start, end), m_numFields(numFields)
			{}
			const char* get_buf() const { return(m_buffer.size() ? &m_buffer.at(0) : NULL); }
			size_t get_size() const { return m_buffer.size(); }
			boost::uint_least16_t get_fields_num() const { return m_numFields; }
		private:
			std::vector<char> 				m_buffer;
			boost::uint_least16_t			m_numFields;
		};

		typedef boost::shared_ptr<SessionBuffer  > SessionBufferPtr;



	} //buff_cache


}// namespace other_fix
#endif
