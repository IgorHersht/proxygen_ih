#ifndef FIX_ESPD_MAPER_H
#define FIX_ESPD_MAPER_H

#include <boost/cstdint.hpp>
#include <boost/bimap.hpp>
#include "fixI.h"
#include "espdI.h"
#include "fix_consts.h"
#include "session_settings_wrapper.h"
#include "msg_converter_I.h"



namespace other_fix{

/*
template <int v1 =  FIX::FIELD::INVALID_TAG, int v2 = FIX::FIELD::INVALID_TAG, int v3 = FIX::FIELD::INVALID_TAG >
    struct TagT {   enum {val1 = v1, val2 = v2, val3 = v3 };  };

template <typename From> struct FieldTagBiMap ;


#define T42_F_MAP11(fld, tag)                                                       \
        template <> struct FieldTagBiMap< FieldT< fld > >                       \
        {                                                                               \
            enum {                                                                      \
                val1 = consts42::TagT<consts42::tag>::val1,                                 \
                val2 = consts42::INVALID_TAG,                                               \
                val3 = consts42::INVALID_TAG                                                \
            };                                                                          \
        };                                                                              \
        template <> struct FieldTagBiMap< consts42::TagT<consts42::tag> > {                 \
            enum {                                                                      \
                val1 = FieldT<fld> ::val1,                                      \
                val2 =  consts::INVALID_FIELD_ID,                                           \
                val3 = consts::INVALID_FIELD_ID                                                 \
            };                                                                          \
        }



#define GET_T42_11(fld) other_fix::FieldTagBiMap< other_fix::FieldT< fld >  > :: val1
#define GET_FLD_11(tag) other_fix::FieldTagBiMap<other_fix::consts42::TagT<other_fix::consts42::tag> > :: val1


T42_F_MAP11(CFETP_FIELD_AVG_PRICE, AvgPx);
T42_F_MAP11(CFETP_FIELD_INSTNAME, Symbol);//
T42_F_MAP11(CFETP_FIELD_CLIENT_ORDER_ID, ClOrdID);
T42_F_MAP11(CFETI_EXT_PROPERTY_ORDER_TYPE, OrdType);
T42_F_MAP11(CFETI_EXT_PROPERTY_ORDER_TIME_TYPE, TimeInForce );
T42_F_MAP11(CFETI_EXT_PROPERTY_CUSTOMER_ACCOUNT, Account );
T42_F_MAP11(CFETP_FIELD_SOURCE_ID, SenderSubID );


*/
struct EspdFixBiMap : private boost::noncopyable{
    typedef boost::bimap<char, boost::uint_least8_t> SideIndicatorT;
    typedef boost::bimap<char , boost::uint_least32_t> OrdTypeT;
    typedef boost::bimap< char, boost::uint_least32_t> TimeInForceT;

    EspdFixBiMap(){
        //side
        m_side_indicator.insert(SideIndicatorT::value_type(FIX::Side_BUY,   CFETI_ORDER_BUY));
        m_side_indicator.insert(SideIndicatorT::value_type(FIX::Side_SELL,   CFETI_ORDER_SELL));
        //OrdTypeT
        m_ordType.insert(OrdTypeT::value_type(FIX::OrdType_MARKET,                  CFETI_EXT_PROPERTY_MARKETORDER));
        m_ordType.insert(OrdTypeT::value_type(FIX::OrdType_LIMIT,                   CFETI_EXT_PROPERTY_LIMITORDER));
        m_ordType.insert(OrdTypeT::value_type(FIX::OrdType_STOP,                    CFETI_EXT_PROPERTY_STOPORDER));
        m_ordType.insert(OrdTypeT::value_type(FIX::OrdType_STOP_LIMIT,              CFETI_EXT_PROPERTY_STOPLIMITORDER));
        m_ordType.insert(OrdTypeT::value_type(FIX::OrdType_MARKET_IF_TOUCHED,       CFETI_EXT_PROPERTY_MARKETIFTOUCHEDORDER));
        m_ordType.insert(OrdTypeT::value_type(FIX::OrdType_MARKET_WITH_LEFTOVER_AS_LIMIT,       CFETI_EXT_PROPERTY_MARKETLIMITORDER));
        //TimeInForce
        m_timeInForce.insert(TimeInForceT::value_type(FIX::TimeInForce_GOOD_TILL_DATE,      CFETI_EXT_PROPERTY_ORDER_GOOD_TILL_DATE));
        m_timeInForce.insert(TimeInForceT::value_type(FIX::TimeInForce_DAY,                 CFETI_EXT_PROPERTY_ORDER_GOOD_FOR_DAY ));
        m_timeInForce.insert(TimeInForceT::value_type(FIX::TimeInForce_GOODTILLCANCEL,      CFETI_EXT_PROPERTY_ORDER_GOOD_TILL_CANCEL));
        m_timeInForce.insert(TimeInForceT::value_type(FIX::TimeInForce_IMMEDIATEORCANCEL,   CFETI_EXT_PROPERTY_ORDER_IMMEDIATE_OR_CANCEL));
        m_timeInForce.insert(TimeInForceT::value_type(FIX::TimeInForce_FILL_OR_KILL,        CFETI_EXT_PROPERTY_ORDER_FILL_OR_KILL));
    }

    boost::uint_least8_t getIndicator(  char side) const {
        typedef  SideIndicatorT::left_map MapT;
         const MapT &map = m_side_indicator.left;
         MapT::const_iterator pos = map.find(side);
         return (pos != map.end() ) ? pos->second : FIX::Side_INVALID;
    }

    char getSide(  boost::uint_least8_t side) const {
            typedef  SideIndicatorT::right_map MapT;
             const MapT &map = m_side_indicator.right ;
             MapT::const_iterator pos = map.find(side);
             return (pos != map.end() ) ? pos->second : FIX::Side_INVALID;
    }

    boost::uint_least32_t getOrderType(  char type) const {
         typedef  OrdTypeT::left_map MapT;
         const MapT &map = m_ordType.left;
         MapT::const_iterator pos = map.find(type);
         return (pos != map.end() ) ? pos->second : FIX::OrdType_INLALID;
    }

    boost::uint_least32_t getTimeInForce( char timeInForce) const {
         typedef  TimeInForceT::left_map MapT;
         const MapT &map = m_timeInForce.left;
         MapT::const_iterator pos = map.find(timeInForce);
         return (pos != map.end() ) ? pos->second : FIX::TimeInForce_INVALID;
}

private:
    SideIndicatorT  m_side_indicator;
    OrdTypeT        m_ordType;
    TimeInForceT    m_timeInForce;
};

const EspdFixBiMap  ESPD_FIX_MAP;


template <boost::uint_least16_t msgType> struct  EspdMsgType{
    enum{type = msgType, flag = ' '};
};


struct FixMsgWr : private boost::noncopyable{

    typedef FIX::Message value_type;
    FixMsgWr():m_extSesId(0), m_tsId(0){}
    explicit FixMsgWr(const FIX::Message &msg): m_msg(msg), m_extSesId(0), m_tsId(0){}

    void set_extSesId(boost::uint_least32_t extSesId ) {m_extSesId =  extSesId ; }
    void set_tsId(TradingSystemId tsId ) {m_tsId =  tsId ; }
    void setMsg(const FIX::Message &msg){ m_msg = msg; }

    FIX::Message& getMsg() { return m_msg ;}
    const FIX::SessionID& get_sessionID(const SessionsInfo &config) const{
          return config.getSessionId(TS_SessionIdT( m_extSesId, m_tsId));
    }
    
private:
    FIX::Message                    m_msg;
    boost::uint_least32_t           m_extSesId;
    TradingSystemId                 m_tsId;
};

template <typename MsgT> struct FixMsgTypeMap;

template <> struct FixMsgTypeMap <FIX42::Logon>{
    enum { protocol =                           FIX_42};
    typedef FixMsgLogon                         fix_type;
    typedef FIX42::Logon                        fix_protocol_type;
    typedef EspdMsgType<CFETP_CONNECT>          espd_type;
    enum  {converter_src_target =                   FIX_4_2_ST };
};

template <> struct FixMsgTypeMap <FIX42::NewOrderSingle>{
    enum { protocol =                               FIX_42};
    typedef FixMsgNewOrderSingle                    fix_type;
    typedef FIX42::NewOrderSingle                   fix_protocol_type;
    typedef EspdMsgType<CFETP_POST_MESSAGE>         espd_type;
    enum  {converter_src_target =                   FIX_4_2_ST };
};

template <> struct FixMsgTypeMap < FIX42::OrderCancelReplaceRequest >{
    enum { protocol =                               FIX_42 };
    typedef FixMsgOrderCancelReplaceRequest         fix_type;
    typedef FIX42::OrderCancelReplaceRequest        fix_protocol_type;
    typedef EspdMsgType<CFETP_POST_MESSAGE>         espd_type;
    enum  {converter_src_target =                   FIX_4_2_ST };
};

template <> struct FixMsgTypeMap < FIX42::ExecutionReport > {
    enum { protocol =                               FIX_42};
    typedef FixMsgExecutionReport                   fix_type;
    typedef FIX42::ExecutionReport                  fix_protocol_type;
    typedef EspdMsgType< CFETP_NOTIFY>              espd_type;
    enum  {converter_src_target =                   FIX_4_2_ST };
};

template <> struct FixMsgTypeMap < FixMsgProtocolType <FIX_42, FixMsgExecutionReport> > {
    enum { protocol =                               FIX_42};
    typedef FixMsgExecutionReport                   fix_type;
    typedef FIX42::ExecutionReport                  fix_protocol_type;
    typedef EspdMsgType< CFETP_NOTIFY>              espd_type;
    enum  {converter_src_target =                   FIX_4_2_ST };
};


//FixMsgExecutionReport;
}//
#endif
