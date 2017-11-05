#ifndef _MSG_HEADER_H
#define _MSG_HEADER_H
#include "endian_wrapper.h"
#include "espdI.h"

namespace other_fix {


	struct MsgHeader {

		MsgHeader(boost::uint_least32_t sequenceNo, boost::uint_least16_t messageType, boost::uint_least16_t messageFlags,
			boost::uint_least16_t numFields, boost::uint_least16_t length) :
			m_protocolId(CFETP_ID),
			m_version(CFETP_VERSION),
			m_sequenceNo(sequenceNo),
			m_messageType(messageType),
			m_messageFlags(messageFlags),
			m_numFields(numFields),
			m_length(length) {}

		boost::uint_least16_t 	get_protocolId() const { return m_protocolId; }
		boost::uint_least16_t 	get_version() const { return m_version; }
		boost::uint_least32_t 	get_sequenceNo() const { return m_sequenceNo; }
		boost::uint_least16_t 	get_messageType() const { return m_messageType; }
		boost::uint_least8_t 	get_messageFlags() const { return m_messageFlags; }
		boost::uint_least16_t	get_numFields() const { return m_numFields; }
		boost::uint_least16_t 	get_length() const { return m_length; }

	private:
		uespd16_t	m_protocolId;
		uespd16_t 	m_version;
		uespd32_t 	m_sequenceNo;
		uespd16_t 	m_messageType;
		uespd8_t  	m_messageFlags;
		uespd16_t 	m_numFields;
		uespd16_t 	m_length;

	};



}// namespace other_fix







#endif
