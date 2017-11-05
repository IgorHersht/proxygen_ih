#ifndef _MSG_FIELD_TYPE_H
#define _MSG_FIELD_TYPE_H
#include <string.h>
#include "espdI.h"
#include "endian_wrapper.h"
#include "fix_utils.h"


namespace other_fix {


	template <boost::uint_least16_t v1 = consts::INVALID_FIELD_ID, boost::uint_least16_t v2 = consts::INVALID_FIELD_ID, boost::uint_least16_t v3 = consts::INVALID_FIELD_ID>
	struct FieldT { enum { val1 = v1, val2 = v2, val3 = v3 }; };


	enum { INVALID_FLD_SIZE = 0 };

	template <int type> struct MsgFieldT;

	template <> struct MsgFieldT<CFETI_FIELDTYPE_INT8> {
		enum { fixed_size = true };
		enum { value = CFETI_FIELDTYPE_INT8 };
		typedef boost::int_least8_t native_type;
		typedef espd8_t             espd_type;
	};
	template <> struct MsgFieldT<CFETI_FIELDTYPE_BYTE> {
		enum { fixed_size = true };
		enum { value = CFETI_FIELDTYPE_BYTE };
		typedef boost::uint_least8_t    native_type;
		typedef uespd8_t                espd_type;
	};
	template <> struct MsgFieldT<CFETI_FIELDTYPE_INT16> {
		enum { fixed_size = true };
		enum { value = CFETI_FIELDTYPE_INT16 };
		typedef boost::int_least16_t    native_type;
		typedef espd16_t                espd_type;
	};
	template <> struct MsgFieldT<CFETI_FIELDTYPE_UINT16> {
		enum { fixed_size = true };
		enum { value = CFETI_FIELDTYPE_UINT16 };
		typedef boost::uint_least16_t   native_type;
		typedef uespd16_t               espmaxSized_type;
	};
	template <> struct MsgFieldT<CFETI_FIELDTYPE_INT32> {
		enum { fixed_size = true };
		enum { value = CFETI_FIELDTYPE_INT32 };
		typedef boost::int_least32_t    native_type;
		typedef espd32_t                espd_type;
	};
	template <> struct MsgFieldT<CFETI_FIELDTYPE_UINT32> {
		enum { fixed_size = true };
		enum { value = CFETI_FIELDTYPE_UINT32 };
		typedef boost::uint_least32_t native_type;
		typedef uespd32_t           espd_type;
	};
	template <> struct MsgFieldT<CFETI_FIELDTYPE_DATETIME> {
		enum { fixed_size = true };
		enum { value = CFETI_FIELDTYPE_UINT32 };
		typedef boost::uint_least32_t native_type;
		typedef uespd32_t           espd_type;
	};

	template <> struct MsgFieldT<CFETI_FIELDTYPE_DECIMAL > {
		enum { fixed_size = true };
		enum { value = CFETI_FIELDTYPE_DECIMAL };
		typedef double               native_type;
		typedef bespd64_t            espd_type;
	};
	template <> struct MsgFieldT<CFETI_FIELDTYPE_STRING> {
		enum { fixed_size = false };
		enum { value = CFETI_FIELDTYPE_STRING };
		typedef std::string         native_type;
		typedef std::string         espd_type;
	};
	template <> struct MsgFieldT<CFETI_FIELDTYPE_BUFFER> {
		enum { fixed_size = false };
		enum { value = CFETI_FIELDTYPE_BUFFER };
		typedef std::string         native_type;
		typedef std::string         espd_type;

	};

	template <> struct MsgFieldT<CFETI_FIELDTYPE_BYTESTREAM> {
		enum { fixed_size = false };
		enum { value = CFETI_FIELDTYPE_BYTESTREAM };
		typedef std::string         native_type;
		typedef std::string         espd_type;

	};

	template <> struct MsgFieldT<CFETI_FIELDTYPE_EMPTY> {
		enum { fixed_size = true };
		enum { value = CFETI_FIELDTYPE_EMPTY };
		typedef void                native_type;
		typedef void                espd_type;
	};
	typedef struct MsgFieldT<CFETI_FIELDTYPE_EMPTY> InvalidMsgFieldT;

	template<int id> struct IdType;

	template<> struct IdType< CFETP_FIELD_ORIG_CLIENT_ORDER_ID >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_STRING> type;        enum { id = CFETP_FIELD_ORIG_CLIENT_ORDER_ID };
	};
	template<> struct IdType< CFETP_FIELD_SESSIONID >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_STRING> type;        enum { id = CFETP_FIELD_SESSIONID };
	};
	template<> struct IdType< CFETP_FIELD_TS_ID >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_UINT32 > type;      enum { id = CFETP_FIELD_TS_ID };
	};
	template<> struct IdType< CFETP_FIELD_TS_PREF >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_UINT32> type;       enum { id = CFETP_FIELD_TS_PREF };
	};
	template<> struct IdType< CFETP_FIELD_PASSWORD >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_STRING> type;       enum { id = CFETP_FIELD_PASSWORD };
	};
	template<> struct IdType< CFETP_FIELD_USERNAME >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_STRING> type;       enum { id = CFETP_FIELD_USERNAME };
	};
	template<> struct IdType< CFETP_FIELD_GMT_DELTA >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_INT32> type;        enum { id = CFETP_FIELD_GMT_DELTA };
	};
	template<> struct IdType< CFETP_FIELD_IDENT_COMPANY >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_STRING> type;       enum { id = CFETP_FIELD_IDENT_COMPANY };
	};
	template<> struct IdType< CFETP_FIELD_IDENT_APPLICATION >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_STRING> type;       enum { id = CFETP_FIELD_IDENT_APPLICATION };
	};
	template<> struct IdType< CFETP_FIELD_IDENT_VERSION>
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_STRING> type;       enum { id = CFETP_FIELD_IDENT_VERSION };
	};
	template<> struct IdType< CFETP_FIELD_COMPONENT_TYPE >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_UINT32> type;       enum { id = CFETP_FIELD_COMPONENT_TYPE };
	};
	template<> struct IdType< CFETP_FIELD_IDENT_USER_OS >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_STRING> type;       enum { id = CFETP_FIELD_IDENT_USER_OS };
	};
	template<> struct IdType< CFETP_FIELD_IDENT_SDK_VERSION_ID >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_UINT32> type;       enum { id = CFETP_FIELD_IDENT_SDK_VERSION_ID };
	};
	template<> struct IdType< CFETP_FIELD_EXT_TS_SESSIONID >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_UINT32> type;       enum { id = CFETP_FIELD_EXT_TS_SESSIONID };
	};
	template<> struct IdType< CFETP_FIELD_SOURCE_ID>
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_STRING> type;       enum { id = CFETP_FIELD_SOURCE_ID };
	};
	template<> struct IdType< CFETP_FIELD_INSTNAME>
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_STRING> type;       enum { id = CFETP_FIELD_INSTNAME };
	};
	template<> struct IdType< CFETP_FIELD_NUM_PREFERENCES>
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_UINT32> type;       enum { id = CFETP_FIELD_NUM_PREFERENCES };
	};
	template<> struct IdType< CFETP_FIELD_CLIENT_ORDER_ID >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_STRING> type;       enum { id = CFETP_FIELD_CLIENT_ORDER_ID };
	};
	template<> struct IdType< CFETP_FIELD_PRICE>
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_DECIMAL> type;      enum { id = CFETP_FIELD_PRICE };
	};
	template<> struct IdType< CFETP_FIELD_SIZE>
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_DECIMAL> type;      enum { id = CFETP_FIELD_SIZE };
	};
	template<> struct IdType< CFETP_FIELD_INDICATOR>
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_BYTE> type;         enum { id = CFETP_FIELD_INDICATOR };
	};
	template<> struct IdType< CFETP_FIELD_PREFERENCE_ID>
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_UINT32> type;           enum { id = CFETP_FIELD_PREFERENCE_ID };
	};
	template<> struct IdType< CFETP_FIELD_PREFERENCE_TYPE>
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_UINT32> type;           enum { id = CFETP_FIELD_PREFERENCE_TYPE };
	};

	template<> struct IdType< CFETP_FIELD_ORDER_INFO_TYPE>
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_UINT32> type;       enum { id = CFETP_FIELD_ORDER_INFO_TYPE };
	};
	template<> struct IdType< CFETP_FIELD_CMDPREF >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_UINT32> type;       enum { id = CFETP_FIELD_CMDPREF };
	};
	template<> struct IdType< CFETP_FIELD_TRADE_PREF2 >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_UINT32> type;       enum { id = CFETP_FIELD_TRADE_PREF2 };
	};

	struct FldId {
		explicit FldId(boost::uint_least16_t id) :m_id(id) {}
		boost::uint_least16_t get() const { return m_id; }
		boost::uint_least16_t get_espd() const { return *((boost::uint_least16_t*)&(m_id)); }
	private:
		uespd16_t               m_id;
	};

	struct FldType {
		FldType(boost::uint_least8_t type) :m_type(type) {}
		boost::uint_least8_t get()  const { return m_type; }
		boost::uint_least8_t get_espd()const { return *((boost::uint_least8_t*)&(m_type)); }
	private:
		uespd8_t                m_type;
	};

	struct FldHdr {
		static boost::uint_least16_t get_id(const char* fld) {
			const  FldHdr  *hdr = (const FldHdr *)fld;
			return hdr->get_id();
		}
		static boost::uint_least8_t get_type(const char* fld) {
			const  FldHdr  *hdr = (const FldHdr *)fld;
			return hdr->get_type();
		}
		//FldHdr():m_id(0), m_type(0){}
		FldHdr(boost::uint_least16_t id, boost::uint_least8_t type) :m_id(id), m_type(type) {}

		boost::uint_least16_t get_id()          const { return m_id.get(); }
		boost::uint_least8_t get_type()         const { return m_type.get(); }
		boost::uint_least16_t get_id_espd()     const { return m_id.get_espd(); }
		boost::uint_least8_t get_type_espd()    const { return m_type.get_espd(); }
	private:
		FldId                   m_id;
		FldType                 m_type;
	};

	namespace fld_consts {

		template<boost::uint_least16_t id, boost::uint_least8_t type = IdType<id>::type::value>
		struct FldHdrValue {
			const static FldHdr value;
		};
		template<boost::uint_least16_t id, boost::uint_least8_t type> const FldHdr FldHdrValue<id, type> ::value(id, type);

		template<boost::uint_least16_t id> struct FldIdValue {
			const static FldId value;
			const static boost::uint_least16_t id_espd;
		};
		template<boost::uint_least16_t id> const FldId FldIdValue<id> ::value(id);
		template<boost::uint_least16_t id> const boost::uint_least16_t FldIdValue<id> ::id_espd((*(boost::uint_least16_t*)&value));

	}// namespace fld_consts

	template <typename T> struct MsgField {
		BOOST_STATIC_ASSERT(T::fixed_size == true);
		BOOST_STATIC_ASSERT(T::value != CFETI_FIELDTYPE_EMPTY);
		static boost::uint_least16_t get_id(const char* fld) {
			MsgField *msgField = (MsgField *)fld;
			return msgField->get_id();
		}
		static boost::uint_least8_t get_type(const char* fld) {
			MsgField *msgField = (MsgField *)fld;
			return msgField->get_type();
		}
		static typename T::native_type get_data(const char* fld) {
			MsgField *msgField = (MsgField *)fld;
			return msgField->get_data();
		}
		MsgField() :m_hdr(0, 0), m_data(typename T::native_type()) {}
		MsgField(boost::uint_least16_t id, boost::uint_least8_t type, const typename T::native_type &data) :
			m_hdr(id, type), m_data(data) {}
		MsgField(boost::uint_least16_t id, const typename T::native_type &data) :
			m_hdr(id, T::value), m_data(data) {}
		MsgField(const FldHdr &hdr, typename T::native_type data) :
			m_data(data) {
			memcpy(&m_hdr, &hdr, sizeof(FldHdr));
		}
		boost::uint_least16_t get_id()      const { return m_hdr.get_id(); }
		boost::uint_least8_t get_type()     const { return m_hdr.get_type(); }
		typename T::native_type get_data()  const { return m_data; }
	private:
		FldHdr                  m_hdr;
		typename T::espd_type   m_data;
	};


	template <> struct MsgField< MsgFieldT< CFETI_FIELDTYPE_STRING> > {
		typedef MsgFieldT<CFETI_FIELDTYPE_STRING> T;
		static boost::uint_least16_t get_id(const char* fld) {
			MsgField *msgField = (MsgField *)fld;
			return msgField->get_id();
		}
		static boost::uint_least8_t get_type(const char* fld) {
			MsgField *msgField = (MsgField *)fld;
			return msgField->get_type();
		}
		static  T::native_type get_data(const char* fld) {
			MsgField *msgField = (MsgField *)fld;
			return msgField->get_data();
		}

		MsgField(boost::uint_least16_t id) :
			m_hdr(id, T::value) {
			m_data_start[0] = '\0';
		}
		enum { FIXED_SIZE = sizeof(boost::uint_least16_t) + sizeof(boost::uint_least8_t) + 1 };

		boost::uint_least16_t get_id()      const { return m_hdr.get_id(); }
		boost::uint_least8_t get_type()     const { return m_hdr.get_type(); }
		T::native_type       get_data()  const { return ((char*)m_data_start); }

	private:
		FldHdr                  m_hdr;
		char                    m_data_start[1];
	};




	template <typename FldT> struct FieldFixedSize {
		BOOST_STATIC_ASSERT(FldT::fixed_size == true);
		enum { SIZE = sizeof(FldHdr) + sizeof(typename FldT::native_type) };
	};

	inline int strFldSize(const char *fld, size_t maxSize) {
		if (!fld) return consts::INVALID_FLD_SZ;
		if (maxSize <= sizeof(FldHdr)) return consts::INVALID_FLD_SZ;
		const char *data = fld + sizeof(FldHdr);

		char *end = (char *)memchr(data, '\0', maxSize);
		if (!end) return consts::INVALID_FLD_SZ;
		return   (end - fld) + 1;

	}

	struct  Order_Preferences {
		Order_Preferences() :m_cmdRef(0), m_tradePref2(0) {}


		void addPreferenceFlag(boost::uint_least32_t flag) {
			m_cmdRef = setPreferencesFlag(m_cmdRef, flag);
		}
		void addPreferenceFlag2(boost::uint_least32_t flag) {
			m_tradePref2 = setPreferencesFlag(m_tradePref2, flag);
		}
		boost::uint_least32_t getPreference() const { return m_cmdRef; }
		boost::uint_least32_t getPreference2() const { return m_tradePref2; }
	private:
		boost::uint_least32_t m_cmdRef; //CFETP_FIELD_CMDPREF
		boost::uint_least32_t m_tradePref2; //CFETP_FIELD_TRADE_PREF2
	};

	struct  Order_PrefFlds {
		typedef boost::tuple<
			MsgField< MsgFieldT< CFETI_FIELDTYPE_UINT32 > >,
			MsgField< MsgFieldT< CFETI_FIELDTYPE_UINT32 > >
		> FieldsT;
		enum { FLD_NUM = boost::tuples::length<FieldsT>::value };

		Order_PrefFlds(const Order_Preferences &pref) :
			m_fields(
				MsgField< MsgFieldT< CFETI_FIELDTYPE_UINT32 > >(CFETP_FIELD_CMDPREF, pref.getPreference()),
				MsgField< MsgFieldT< CFETI_FIELDTYPE_UINT32 > >(CFETP_FIELD_TRADE_PREF2, pref.getPreference2())
			)
		{}

	private:
		FieldsT  m_fields;

	};



}// namespace other_fix
#endif
