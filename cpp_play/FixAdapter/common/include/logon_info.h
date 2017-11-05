#ifndef _LOGON_INFO_H
#define _LOGON_INFO_H
#include <string>
#include <map>
#include "fixI.h"
#include "fix_utils.h"
#include "espdI.h"

namespace other_fix{

struct SessionLogonInfo : private boost::noncopyable{
    SessionLogonInfo(boost::uint_least32_t tsPref,  const std::string  &sessId,
            const std::string  &userName,const std::string  &passWord,
            TradingSystemId tsId):
             m_tsPref(tsPref), m_sessId(sessId),
            m_userName(userName), m_passWord(passWord),
            m_tsId(tsId) { }

    void set_xsessId(boost::uint_least32_t xsessId ){ m_xsessId = xsessId; }
    void set_tsId(boost::uint_least32_t tsId ){ m_tsId =  tsId; }

    TradingSystemId      get_tsId()const { return  m_tsId; }
    boost::uint_least32_t get_tsPref( )const {  return  m_tsPref; }
    const std::string& get_sessId()const {  return  m_sessId; }
    const std::string& get_userName( )const {   return  m_userName; }
    const std::string& get_passWord()const {    return  m_passWord; }
    boost::uint_least32_t get_xsessId( TradingSystemId ts_id )const {  return  m_xsessId; }
private:
    boost::uint_least32_t       m_tsPref;
    std::string                 m_sessId;
    std::string                 m_userName;
    std::string                 m_passWord;
    TradingSystemId             m_tsId;
    boost::uint_least32_t       m_xsessId;
};

}// namespace other_fix
#endif
