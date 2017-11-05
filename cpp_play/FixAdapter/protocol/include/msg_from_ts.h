#ifndef MSG_FROM_TS_H
#define MSG_FROM_TS_H


#include "session_settings_wrapper.h"
#include "fix_espd_maper.h"
#include "ts_notify_manager.h"
#include "msg_from_ts_parser.h"


namespace other_fix{


struct MsgFromTs : public MsgFromTSParser, private boost::noncopyable{
    typedef SessionsInfo ArgT;
    typedef MsgFromTSParser Base;
    typedef std::map<FIX::SessionID , UniqueGeneratorPtr > UniqGensT;
    explicit MsgFromTs(const SessionsInfo& config):
        Base(config), m_config(config),  m_notifyMgr(config, getFields() ){
             const SessionsInfo::SessinsSettingsT& ses = config.getSessionsSettings() ;
            for(SessionsInfo::SessinsSettingsT::const_iterator pos = ses.begin(), end = ses.end(); pos != end; ++pos ){
                m_gens.insert(UniqGensT::value_type( pos->first, UniqueGeneratorPtr(new UniqueGenerator )) );
            }
        }

void convert(NullType, FixMessageInfo &fixMsg) {
       const  boost::uint_least16_t msgType = get_msgType();
       manageEspdMsg(fixMsg, msgType);;
}
private:

void manageEspdMsg(FixMessageInfo &fixMsg, boost::uint_least16_t msgType){
     
    switch (msgType)
    {
        case CFETP_NOTIFY:          return manageEspdMsg(EspdMsgType< CFETP_NOTIFY>(), fixMsg );
        case CFETP_RATIFY:          return manageEspdMsg(EspdMsgType< CFETP_RATIFY>(), fixMsg );
        case CFETP_ROUNDTRIP:       return manageEspdMsg(EspdMsgType< CFETP_ROUNDTRIP>(), fixMsg );
        case CFETP_CONNECT_REPLY:   return manageEspdMsg(EspdMsgType< CFETP_CONNECT_REPLY>(), fixMsg );
        case CFETP_TRADING_SESSION: return manageEspdMsg(EspdMsgType<  CFETP_TRADING_SESSION >(), fixMsg );
        case CFETP_COMPONENT:       return manageEspdMsg(EspdMsgType< CFETP_COMPONENT>(), fixMsg );
        case CFETP_SYSTEM_REPLY:    return manageEspdMsg(EspdMsgType< CFETP_SYSTEM_REPLY>() , fixMsg );
    }  
    std::string err("Unknown  message type "); err += boost::lexical_cast<std::string>(msgType);
    err+= ".";
    warningxxx(err);
    
}

  
void  manageEspdMsg(EspdMsgType< CFETP_NOTIFY>, FixMessageInfo &fixMsg){
    boost::uint_least32_t  cmd = findFieldData< MsgFieldT< CFETI_FIELDTYPE_UINT32 > >(CFETP_FIELD_CMD);
    m_notifyMgr.manageEspdMsg( cmd, fixMsg);
    if( fixMsg.first && fixMsg.second) {
            m_notifyMgr.setExecId(getNextUniqStr(*fixMsg.second));
    }
}

void  manageEspdMsg(EspdMsgType< CFETP_CONNECT_REPLY> , FixMessageInfo ){

    std::string sessionIDStr;
    boost::uint_least32_t xsessId = 0;
    boost::uint_least32_t tsId = 0;
    // TODO fix double parsing here
    for(const char *pos = firstFld(); pos != NULL; pos = nextFld() ){
        if(!pos) {
            throw ESPDMsgError("Connect  message parsing error.");
        }
        const boost::uint_least16_t id = FldHdr::get_id(pos);
        switch (id)
        {
            case CFETP_FIELD_SESSIONID:{
                sessionIDStr= MsgField< IdType< CFETP_FIELD_SESSIONID >:: type > ::get_data(pos);
                break;
                }
            case  CFETP_FIELD_EXT_TS_SESSIONID:
                xsessId = MsgField< IdType< CFETP_FIELD_EXT_TS_SESSIONID >:: type > ::get_data(pos);
                break;
            case  CFETP_FIELD_TS_ID:
                tsId = MsgField< IdType< CFETP_FIELD_TS_ID >:: type > ::get_data(pos);
                break;
        };
    }
   
    if(sessionIDStr.empty() ) {
        throw FIXError(" Cannot set SessionID. The message is ignored. ");
    }
    if( !xsessId ){
        throw FIXError(" Cannot set xsessId. The message is ignored. ");
    }
    if( !tsId ){
         throw FIXError(" Cannot set tsId. The message is ignored. ");
    }
    FIX::SessionID sessionID;
    sessionID.fromString( sessionIDStr);
    const bool status = m_config.set_sessId(sessionID, xsessId, tsId  );
    if(!status){
        throw FIXError(" Cannot set session ID " + sessionIDStr);
    }
}

void  manageEspdMsg(EspdMsgType< CFETP_RATIFY>, FixMessageInfo &fixMsg){
    logxxx(" RATIFY  message has not been implemented yet." );
}

void  manageEspdMsg(EspdMsgType< CFETP_ROUNDTRIP> , FixMessageInfo &fixMsg){
     logxxx(" ROUNDTRIP message has not been implemented yet." );
}

void  manageEspdMsg(EspdMsgType< CFETP_TRADING_SESSION> , FixMessageInfo &fixMsg){
      logxxx(" TRADING_SESSION  message has not been implemented yet. " );
}

void manageEspdMsg(EspdMsgType< CFETP_COMPONENT> , FixMessageInfo &fixMsg){
    logxxx(" COMPONENT e message has not been implemented yet. " );
}

void manageEspdMsg(EspdMsgType< CFETP_SYSTEM_REPLY>, FixMessageInfo &fixMsg ){
    logxxx(" SYSTEM_REPLY  message has not been implemented yet. " );
}

private:
std::string getNextUniqStr(const FIX::SessionID& sessionID ) const {
    UniqGensT::const_iterator pos = m_gens.find(sessionID );
    if(pos == m_gens.end()){
        throw ESPDMsgError(" Cannot generate ExecID. Message ignored. " );
    }
    return  pos->second->next();
  }

private:
        const SessionsInfo&             m_config;
        TSNotifyManager                  m_notifyMgr;
        UniqGensT                       m_gens;
};


typedef boost::shared_ptr<MsgFromTs> MsgFromTsPtr;







}// namespace other_fix
#endif
