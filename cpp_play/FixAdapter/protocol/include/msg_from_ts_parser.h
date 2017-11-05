#ifndef  MSG_FROM_TS_PARSER_H
#define  MSG_FROM_TS_PARSER_H

#include <boost/array.hpp>
#include <string>
#include "buffer.h"
#include "msh_field_type.h"
#include "msg_header.h"
#include "logger.h"
#include "session_settings_wrapper.h"
#include "logger.h"



namespace FIX {

	class SessionID;
	class SessionSettings;
}


namespace other_fix {


	struct MsgFromTSParser {
		typedef std::vector<const char *> FieldPosT;
		enum { INIT_FLDS_NUM = 100 }; // 100 should be big enoug not to relocate

		explicit MsgFromTSParser(const SessionsInfo &config) :m_config(config),
			m_bodySize(0),
			m_numFields(0),
			m_msgType(CFETP_UNDEFINED),
			m_currentNumFields(0) {

			m_fields.reserve(INIT_FLDS_NUM);
		}
		const SessionsInfo & getConfig()const { return m_config; }
		char*  get_buf() const { return m_buf->get_buf(); }
		size_t get_offset() const { return  m_buf->get_offset(); }
		char*  get_pos() const { return m_buf->get_pos(); }
		void set_offset(size_t offset) { m_buf->set_offset(offset); }
		void reset() {
			m_buf->reset();
			m_fields.clear();
			if (m_fields.capacity() < INIT_FLDS_NUM) {// should not happened
				m_fields.reserve(INIT_FLDS_NUM);
			}
			m_bodySize = 0;
			m_numFields = 0;
			m_msgType = CFETP_UNDEFINED;
		}
		void reset(BufferPtr buf) {
			m_buf = buf;
			m_fields.clear();
			if (m_fields.capacity() < INIT_FLDS_NUM) {// should not happened
				m_fields.reserve(INIT_FLDS_NUM);
			}
			m_bodySize = 0;
			m_numFields = 0;
			m_msgType = CFETP_UNDEFINED;
		}

		size_t      get_bodySize() const { return m_bodySize; }
		boost::uint_least16_t  get_msgType() const { return  m_msgType; }
		void setHeader() {
			const MsgHeader* hdr = (const MsgHeader*)get_buf();
			m_bodySize = hdr->get_length();
			m_numFields = hdr->get_numFields();
			m_msgType = hdr->get_messageType();
		}
		void parseBuffer() {
			setHeader();
			parseBody();
		}

	private:
		int nextFldSize(size_t maxSize) const {
			const char *fld = get_pos();
			const  boost::uint_least8_t   type = FldHdr::get_type(fld);
			switch (type) {
			case CFETI_FIELDTYPE_UINT32:
			case CFETI_FIELDTYPE_INT32:
			case CFETI_FIELDTYPE_DATETIME:
				return  FieldFixedSize< MsgFieldT <CFETI_FIELDTYPE_INT32> >::SIZE;
			case CFETI_FIELDTYPE_DECIMAL:
				return FieldFixedSize< MsgFieldT <CFETI_FIELDTYPE_DECIMAL> >::SIZE;
			case CFETI_FIELDTYPE_STRING:
				return strFldSize(fld, maxSize);
			case CFETI_FIELDTYPE_BYTE:
			case CFETI_FIELDTYPE_INT8:
				return  FieldFixedSize< MsgFieldT <CFETI_FIELDTYPE_INT8> >::SIZE;
			case CFETI_FIELDTYPE_UINT16:
			case CFETI_FIELDTYPE_INT16:
				return FieldFixedSize< MsgFieldT <CFETI_FIELDTYPE_INT16> >::SIZE;
			};
			return consts::INVALID_FLD_SZ;
		}
	public:
		const char* firstFld() {
			set_offset(sizeof(MsgHeader));
			m_currentNumFields = 0;
			return  get_pos();
		}

		const char*   nextFld() {
			const int max_size = m_bodySize + sizeof(MsgHeader) - get_offset();
			if (max_size <= (int)sizeof(FldHdr)) {
				warningxxx("Invalid max field size.");
				return NULL;
			}

			const int size = nextFldSize(max_size);
			if (size == consts::INVALID_FLD_SZ) {// cannot calculate size
				warningxxx("Field size cannot be calculated.");
				return NULL;
			}
			else if (size < max_size) {// OK return next field
				m_currentNumFields++;
				set_offset(get_offset() + size);
				return get_pos();
			}
			else if (size == max_size) {//OK was the last field
				m_currentNumFields++;
				if (m_currentNumFields != m_numFields) {//last one
					warningxxx("Discrepancy between message size and max field number.");
				}
				return NULL;
			}
			else { // size > max_size
				warningxxx("Field size is too big.");
				return NULL;
			}
		}

		void parseBody() {
			for (const char *pos = firstFld(); pos != NULL; pos = nextFld()) {
				m_fields.push_back(pos);
			}
		}
		const FieldPosT& getFields() const { return m_fields; }

		template <typename T> typename T::native_type findFieldData(boost::uint_least16_t id) const {// Linear search !!!
			typedef typename T::native_type ReturnT;

			for (FieldPosT::const_iterator pos = m_fields.begin(), end = m_fields.end(); pos != end; ++pos) {
				const MsgField<T> *fld = (const MsgField<T> *) (*pos);
				if (!fld) return ReturnT(); //should not happened
				if (fld->get_id() == id) {
					return fld->get_data();
				}
			}
			return ReturnT();
		}
	private:
		const SessionsInfo              &m_config;
		BufferPtr                       m_buf;
		//body message size, set after reading header and socket.
		//set to avoid to many calls to  MsgHeader::get_length()
		size_t                      m_bodySize;
		boost::uint_least16_t       m_numFields;
		boost::uint_least16_t       m_msgType;
		boost::uint_least16_t       m_currentNumFields;
		FieldPosT                   m_fields;
	};

	typedef boost::shared_ptr<MsgFromTSParser> BufferFromTSPtr;

}// namespace other_fix
#endif
