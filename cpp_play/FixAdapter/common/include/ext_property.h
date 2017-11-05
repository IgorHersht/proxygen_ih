#ifndef _EXT_PROPERTY_H
#define _EXT_PROPERTY_H
#include <string.h>
#include "espdI.h"
#include "endian_wrapper.h"
#include "fix_utils.h"
#include "msh_field_type.h"
#include "fix_consts.h"


namespace other_fix {


	// Fix property type map
	template<int pr>  struct PrType;

	template<> struct PrType< CFETI_EXT_PROPERTY_GIVEUP_MEMBER_CLEARING_CODE >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_STRING> type;        enum { id = CFETI_EXT_PROPERTY_GIVEUP_MEMBER_CLEARING_CODE };
	};

	template<> struct PrType< CFETI_EXT_PROPERTY_ACCOUNT_CODE >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_STRING> type;        enum { id = CFETI_EXT_PROPERTY_ACCOUNT_CODE };
	};

	template<> struct PrType< CFETI_EXT_PROPERTY_LABEL >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_STRING> type;        enum { id = CFETI_EXT_PROPERTY_LABEL };
	};

	template<> struct PrType< CFETI_EXT_PROPERTY_CUSTOMER_ACCOUNT >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_STRING> type;        enum { id = CFETI_EXT_PROPERTY_CUSTOMER_ACCOUNT };
	};

	template<> struct PrType< CFETI_EXT_PROPERTY_EXCHANGE_ACCOUNT_NAME >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_STRING> type;        enum { id = CFETI_EXT_PROPERTY_EXCHANGE_ACCOUNT_NAME };
	};

	template<> struct PrType< CFETI_EXT_PROPERTY_ROUTING_DELAY >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_STRING> type;        enum { id = CFETI_EXT_PROPERTY_ROUTING_DELAY };
	};

	template<> struct PrType< CFETI_EXT_PROPERTY_SLED_PRICING_METHOD >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_BYTE> type;      enum { id = CFETI_EXT_PROPERTY_SLED_PRICING_METHOD };
	};

	template<> struct PrType< CFETI_EXT_PROPERTY_ORDER_TYPE >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_UINT32> type;        enum { id = CFETI_EXT_PROPERTY_ORDER_TYPE };
	};

	template<> struct PrType< CFETI_EXT_PROPERTY_ORDER_TIME_TYPE >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_UINT32> type;        enum { id = CFETI_EXT_PROPERTY_ORDER_TIME_TYPE };
	};

	template<> struct PrType< CFETI_EXT_PROPERTY_ORDER_RESTRICTION >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_UINT32> type;        enum { id = CFETI_EXT_PROPERTY_ORDER_RESTRICTION };
	};

	template<> struct PrType< CFETI_EXT_PROPERTY_ORDER_OPENPOSITION >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_UINT32> type;        enum { id = CFETI_EXT_PROPERTY_ORDER_OPENPOSITION };
	};

	template<> struct PrType< CFETI_EXT_PROPERTY_CTI_CODE >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_UINT32> type;        enum { id = CFETI_EXT_PROPERTY_CTI_CODE };
	};

	template<> struct PrType< CFETI_EXT_PROPERTY_ORDER_ORIGIN >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_UINT32> type;        enum { id = CFETI_EXT_PROPERTY_ORDER_ORIGIN };
	};


	template<> struct PrType< CFETI_EXT_PROPERTY_MINIMUM_VOLUME >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_DECIMAL> type;       enum { id = CFETI_EXT_PROPERTY_MINIMUM_VOLUME };
	};

	template<> struct PrType< CFETI_EXT_PROPERTY_CLIP_SIZE >
	{
		typedef MsgFieldT<CFETI_FIELDTYPE_DECIMAL> type;       enum { id = CFETI_EXT_PROPERTY_CLIP_SIZE };
	};

	typedef MsgFieldT< CFETI_FIELDTYPE_UINT32> PrefIdT;
	typedef  MsgFieldT< CFETI_FIELDTYPE_UINT32> PrefTypeT;

	template<typename ValueT, typename IdT, typename TypeT> struct ExtOrderProperty;

	template<>
	struct ExtOrderProperty<boost::uint_least32_t, boost::uint_least32_t, boost::uint_least32_t> {
		ExtOrderProperty() {}
		ExtOrderProperty(boost::uint_least32_t value, boost::uint_least32_t id, boost::uint_least32_t type) :
			m_value(CFETP_FIELD_PREFERENCE_VALUE, CFETI_FIELDTYPE_UINT32, value),
			m_id(CFETP_FIELD_PREFERENCE_ID, CFETI_FIELDTYPE_UINT32, id),
			m_type(CFETP_FIELD_PREFERENCE_TYPE, CFETI_FIELDTYPE_UINT32, type)
		{}
		enum { FLD_NUM = 3 };
	private:
		typedef MsgFieldT< CFETI_FIELDTYPE_UINT32> PrefIdT;
		MsgField<PrefIdT>       m_value;
		MsgField<PrefIdT>       m_id;
		MsgField<PrefIdT>       m_type;

	};
	typedef ExtOrderProperty<boost::uint_least32_t, boost::uint_least32_t, boost::uint_least32_t> FixedSzProperty;




	namespace {

		template<boost::uint_least32_t val, boost::uint_least32_t id, boost::uint_least32_t type = CFETI_FIELDTYPE_UINT32 >
		struct FixedSzPropertyValue {
			const static FixedSzProperty value;
		};
		template<boost::uint_least32_t val, boost::uint_least32_t id, boost::uint_least32_t type>
		const FixedSzProperty FixedSzPropertyValue<val, id, type> ::value(val, id, type);

		typedef MsgField <PrefIdT  > PrefIdFldT;
		typedef MsgField <PrefTypeT  > PrefTypeFldT;

		const std::pair<PrefIdFldT, PrefTypeFldT>
			AccountExtPropetiestIdType(PrefIdFldT(CFETP_FIELD_PREFERENCE_ID, CFETI_EXT_PROPERTY_CUSTOMER_ACCOUNT),
				PrefTypeFldT(CFETP_FIELD_PREFERENCE_TYPE, CFETI_FIELDTYPE_STRING));

	}// namespace consts


	struct ExtPrFixKey {
		ExtPrFixKey() :m_ordType(FIX::OrdType_INLALID), m_timeInForce(FIX::TimeInForce_INVALID) {}
		ExtPrFixKey(char ordType, char timeInForce) : m_ordType(ordType), m_timeInForce(timeInForce) {}
	private:
		friend bool operator <(const ExtPrFixKey &lk, const ExtPrFixKey &rk) {
			if (lk.m_ordType < rk.m_ordType) return true;
			if (lk.m_ordType > rk.m_ordType) return false;
			else return (lk.m_timeInForce < rk.m_timeInForce);
		}
		char m_ordType;
		char m_timeInForce;
	};

	typedef boost::tuple<FixedSzProperty, FixedSzProperty> ExtPrValue;

	class ExtProperties : private boost::noncopyable {

		typedef std::map<ExtPrFixKey, ExtPrValue > PropertiesT;

	public:
		enum { FLD_NUM = FixedSzProperty::FLD_NUM * boost::tuples::length<ExtPrValue>::value };

		const ExtPrValue* find(ExtPrFixKey key)const {
			PropertiesT::const_iterator pos = m_properties.find(key);
			return ((pos != m_properties.end()) ? &(pos->second) : NULL);
		}
		ExtProperties() {
			ExtPrFixKey key; ExtPrValue value;
			// market order
			key = ExtPrFixKey(FIX::OrdType_MARKET, FIX::TimeInForce_DAY);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_MARKETORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_GOOD_FOR_DAY, CFETI_EXT_PROPERTY_ORDER_TIME_TYPE>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			key = ExtPrFixKey(FIX::OrdType_MARKET, FIX::TimeInForce_AT_THE_OPENING);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_MARKETONOPENORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_GOOD_FOR_DAY, CFETI_EXT_PROPERTY_ORDER_TIME_TYPE>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			key = ExtPrFixKey(FIX::OrdType_MARKET, FIX::TimeInForce_IMMEDIATE_OR_CANCEL);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_MARKETORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_IMMEDIATE_OR_CANCEL, CFETI_EXT_PROPERTY_ORDER_RESTRICTION>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			key = ExtPrFixKey(FIX::OrdType_MARKET, FIX::TimeInForce_FILL_OR_KILL);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_MARKETORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_FILL_OR_KILL, CFETI_EXT_PROPERTY_ORDER_RESTRICTION>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			key = ExtPrFixKey(FIX::OrdType_MARKET, FIX::TimeInForce_GOOD_TILL_CANCEL);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_MARKETORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_GOOD_TILL_CANCEL, CFETI_EXT_PROPERTY_ORDER_TIME_TYPE>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			// limit order
			key = ExtPrFixKey(FIX::OrdType_LIMIT, FIX::TimeInForce_DAY);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_LIMITORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_GOOD_FOR_DAY, CFETI_EXT_PROPERTY_ORDER_TIME_TYPE>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			//key = ExtPrFixKey(FIX::OrdType_LIMIT, FIX::TimeInForce_AT_THE_OPENING); will be rejected

			key = ExtPrFixKey(FIX::OrdType_LIMIT, FIX::TimeInForce_IMMEDIATE_OR_CANCEL);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_LIMITORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_IMMEDIATE_OR_CANCEL, CFETI_EXT_PROPERTY_ORDER_RESTRICTION>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			key = ExtPrFixKey(FIX::OrdType_LIMIT, FIX::TimeInForce_FILL_OR_KILL);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_LIMITORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_FILL_OR_KILL, CFETI_EXT_PROPERTY_ORDER_RESTRICTION>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			key = ExtPrFixKey(FIX::OrdType_LIMIT, FIX::TimeInForce_GOOD_TILL_CANCEL);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_LIMITORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_GOOD_TILL_CANCEL, CFETI_EXT_PROPERTY_ORDER_TIME_TYPE>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			// stop order
			key = ExtPrFixKey(FIX::OrdType_STOP, FIX::TimeInForce_DAY);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_STOPORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_GOOD_FOR_DAY, CFETI_EXT_PROPERTY_ORDER_TIME_TYPE>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			//key = ExtPrFixKey(FIX::OrdType_STOP, FIX::TimeInForce_AT_THE_OPENING); will be rejected

			key = ExtPrFixKey(FIX::OrdType_STOP, FIX::TimeInForce_IMMEDIATE_OR_CANCEL);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_STOPORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_IMMEDIATE_OR_CANCEL, CFETI_EXT_PROPERTY_ORDER_RESTRICTION>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			key = ExtPrFixKey(FIX::OrdType_STOP, FIX::TimeInForce_FILL_OR_KILL);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_STOPORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_FILL_OR_KILL, CFETI_EXT_PROPERTY_ORDER_RESTRICTION>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			key = ExtPrFixKey(FIX::OrdType_STOP, FIX::TimeInForce_GOOD_TILL_CANCEL);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_STOPORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_GOOD_TILL_CANCEL, CFETI_EXT_PROPERTY_ORDER_TIME_TYPE>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			// stop limit order
			key = ExtPrFixKey(FIX::OrdType_STOPLIMIT, FIX::TimeInForce_DAY);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_STOPLIMITORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_GOOD_FOR_DAY, CFETI_EXT_PROPERTY_ORDER_TIME_TYPE>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			//key = ExtPrFixKey(FIX::OrdType_STOPLIMIT, FIX::TimeInForce_AT_THE_OPENING); will be rejected

			key = ExtPrFixKey(FIX::OrdType_STOPLIMIT, FIX::TimeInForce_IMMEDIATE_OR_CANCEL);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_STOPLIMITORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_IMMEDIATE_OR_CANCEL, CFETI_EXT_PROPERTY_ORDER_RESTRICTION>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			key = ExtPrFixKey(FIX::OrdType_STOPLIMIT, FIX::TimeInForce_FILL_OR_KILL);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_STOPLIMITORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_FILL_OR_KILL, CFETI_EXT_PROPERTY_ORDER_RESTRICTION>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			key = ExtPrFixKey(FIX::OrdType_STOPLIMIT, FIX::TimeInForce_GOOD_TILL_CANCEL);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_STOPLIMITORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_GOOD_TILL_CANCEL, CFETI_EXT_PROPERTY_ORDER_TIME_TYPE>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			// MIT  order
			key = ExtPrFixKey(FIX::OrdType_MARKET_IF_TOUCHED, FIX::TimeInForce_DAY);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_MARKETIFTOUCHEDORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_GOOD_FOR_DAY, CFETI_EXT_PROPERTY_ORDER_TIME_TYPE>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			//key = ExtPrFixKey(FIX::OrdType_MARKET_IF_TOUCHED, FIX::TimeInForce_AT_THE_OPENING); will be rejected

			key = ExtPrFixKey(FIX::OrdType_MARKET_IF_TOUCHED, FIX::TimeInForce_IMMEDIATE_OR_CANCEL);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_MARKETIFTOUCHEDORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_IMMEDIATE_OR_CANCEL, CFETI_EXT_PROPERTY_ORDER_RESTRICTION>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			key = ExtPrFixKey(FIX::OrdType_MARKET_IF_TOUCHED, FIX::TimeInForce_FILL_OR_KILL);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_MARKETIFTOUCHEDORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_FILL_OR_KILL, CFETI_EXT_PROPERTY_ORDER_RESTRICTION>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			key = ExtPrFixKey(FIX::OrdType_MARKET_IF_TOUCHED, FIX::TimeInForce_GOOD_TILL_CANCEL);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_MARKETIFTOUCHEDORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_GOOD_TILL_CANCEL, CFETI_EXT_PROPERTY_ORDER_TIME_TYPE>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			// MIT LIMIT  order
			key = ExtPrFixKey(FIX::OrdType_MARKET_IF_TOUCHED_LIMIT, FIX::TimeInForce_DAY);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_MITLIMITORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_GOOD_FOR_DAY, CFETI_EXT_PROPERTY_ORDER_TIME_TYPE>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			//key = ExtPrFixKey(FIX::OrdType_MARKET_IF_TOUCHED_LIMIT, FIX::TimeInForce_AT_THE_OPENING); will be rejected

			key = ExtPrFixKey(FIX::OrdType_MARKET_IF_TOUCHED_LIMIT, FIX::TimeInForce_IMMEDIATE_OR_CANCEL);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_MITLIMITORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_IMMEDIATE_OR_CANCEL, CFETI_EXT_PROPERTY_ORDER_RESTRICTION>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			key = ExtPrFixKey(FIX::OrdType_MARKET_IF_TOUCHED_LIMIT, FIX::TimeInForce_FILL_OR_KILL);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_MITLIMITORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_FILL_OR_KILL, CFETI_EXT_PROPERTY_ORDER_RESTRICTION>::value);
			m_properties.insert(PropertiesT::value_type(key, value));

			key = ExtPrFixKey(FIX::OrdType_MARKET_IF_TOUCHED_LIMIT, FIX::TimeInForce_GOOD_TILL_CANCEL);
			value.get<0>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_MITLIMITORDER, CFETI_EXT_PROPERTY_ORDER_TYPE>::value);
			value.get<1>() = FixedSzProperty(FixedSzPropertyValue<CFETI_EXT_PROPERTY_ORDER_GOOD_TILL_CANCEL, CFETI_EXT_PROPERTY_ORDER_TIME_TYPE>::value);
			m_properties.insert(PropertiesT::value_type(key, value));
		}

	private:
		PropertiesT     m_properties;

	};

	namespace ext_consts {
		const ExtProperties EXT_PROPERTIES;
	}

}// namespace other_fix
#endif
