#ifndef _FIX_UTILS_H
#define _FIX_UTILS_H

#include "fixI.h"
#include "utils.h"
#include "thread_wrapper.h"
#include "msg_converter_I.h"

namespace other_fix{

namespace fix_consts{
    const FIX::SessionID    INVALID_SESSION_ID(consts::EMPTY_STR, consts::EMPTY_STR, consts::EMPTY_STR);
}

typedef boost::shared_ptr<FIX::SessionSettings> SessionSettingsPtr;
typedef boost::shared_ptr<FIX::MessageStoreFactory> MessageStoreFactoryPtr;
typedef boost::shared_ptr<FIX::LogFactory> LogFactoryPtr;
typedef boost::shared_ptr<FIX::ThreadedSocketAcceptor> FixAcceptorPtr;

typedef FIX::Message* FixMessagePtr;
typedef const FIX::SessionID* FIXSessionIDPtr;
typedef std::pair<  FixMessagePtr, FIXSessionIDPtr > FixMessageInfo;



class SessionManager;
typedef boost::shared_ptr<SessionManager> SessionManagerPtr;
class  TsConnector;
typedef boost::shared_ptr< TsConnector> TsConnectorPtr;
class TSRequestManager;
typedef boost::shared_ptr<TSRequestManager> TSRequestManagerPtr;
class TSRequestsManager;
typedef boost::shared_ptr< TSRequestsManager> TSRequestsManagerPtr;
class TSResponseManager;
typedef boost::shared_ptr<TSResponseManager> TSResponseManagerPtr;
class Application;
typedef boost::shared_ptr<Application> ApplicationPtr;
class TSResponseManagers;
typedef boost::shared_ptr<TSResponseManagers> TSResponseManagersPtr;
class SessionLogonInfo;
typedef boost::shared_ptr<SessionLogonInfo> SessionLogonInfoPtr;
class LogonInfo;
typedef boost::shared_ptr<LogonInfo> LogonInfoPtr;
class SessionsInfo;
typedef boost::shared_ptr< SessionsInfo  > SessionsInfoPtr;
class TradingSytemsSettings;
typedef boost::shared_ptr<TradingSytemsSettings> TradingSytemsSettingsPtr;
class SettingsWrapper;
typedef boost::shared_ptr<SettingsWrapper> SettingsWrapperPtr;
class TsConnectors;
typedef boost::shared_ptr<TsConnectors> TsConnectorsPtr;


inline bool isSessionIdValid(const FIX::SessionID& sessionID){
    return (fix_consts::INVALID_SESSION_ID == sessionID );
}
inline FIX::SessionID getsSessionId(const std::string &sessionID){
    FIX::SessionID id (fix_consts::INVALID_SESSION_ID );
    id.fromString( sessionID );
    return id;
}

template<typename RT> inline RT  getSettingsValue(const std::string &key, const FIX::Dictionary& dic) {

    const std::string & value = dic.getString(key);
    return boost::lexical_cast< RT>( value);

}


template<typename RT> inline std::pair<RT, bool>  getDictionaryValue(const std::string &key, const FIX::Dictionary& dic) {
    try{
        const std::string & value = dic.getString(key);
        return std::pair<RT, bool>(boost::lexical_cast< RT>( value), true) ;
    } catch(...){
         return std::pair<RT, bool>(  RT(), false);
     }
}

template<> inline std::pair<std::string, bool> getDictionaryValue<std::string>(const std::string &key, const FIX::Dictionary& dic) {
    try{
        return std::pair<std::string, bool>(dic.getString(key), true);      
    } catch(...){
         return std::pair<std::string, bool>(consts::EMPTY_STR,false);
     }
}



template <typename MsgT> struct FixProtocolMap;

template <> struct FixProtocolMap <FIX42::Logon> { enum{ PROTOCOL = FIX_4_2_ST};  };

template <> struct FixProtocolMap <FIX42::NewOrderSingle> { enum{ PROTOCOL = FIX_4_2_ST};  };

template <> struct FixProtocolMap < FIX42::OrderCancelReplaceRequest >{  enum{ PROTOCOL = FIX_4_2_ST}; };


}//other_fix
#endif
