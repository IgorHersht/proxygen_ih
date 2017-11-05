#ifndef _MSG_BUFFER_MAP_H
#define _MSG_BUFFER_MAP_H


#include <map>
#include <boost/cstdint.hpp>
#include <boost/bimap/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>
#include <boost/bimap/set_of.hpp>

#include "msh_field_type.h"
#include "msg_from_ts_parser.h"
#include "fix_espd_maper.h"



namespace other_fix{

struct MsgBufferMap : private boost::noncopyable{
    typedef boost::bimaps::bimap< int,  boost::bimaps::multiset_of<boost::uint_least16_t> >BiMapT;
    typedef  BiMapT::value_type OffsetIdT;
    typedef BiMapT::left_map::const_iterator left_const_iterator;
    typedef BiMapT::right_map::const_iterator right_const_iterator;

    MsgBufferMap():m_buffer(NULL){}
    bool insert(const char  *pos){
        if(!m_buffer) return false;
        if(!pos) return false;
        const int offset = pos - m_buffer;
        if(offset <= 0 ) return false;
        const FldId* fld = (const FldId* )pos;
        const bool status = (m_map.insert( BiMapT::value_type(offset, fld->get_espd() ) )).second ;
        return status;
    }


    template<boost::uint_least16_t id, boost::uint_least8_t type> bool findFld( typename MsgFieldT<type>::native_type &value
            ,typename boost::enable_if<boost::is_arithmetic< typename MsgFieldT<type>::native_type> >::type* dummy = 0 )const {
        const char* pos = findFldStart<id>();
        if(!pos) return false;
        MsgField< MsgFieldT<type> > *fld =  (MsgField< MsgFieldT<type> > *)pos;
        value = fld->get_data();
        return true;
    }
    template<boost::uint_least16_t id> const char* findFld()const {
            const char* pos = findFldStart<id>();
            if(!pos) return NULL;
            return pos + sizeof(FldHdr);
        }

    void reset(const char*  buffer){
        m_buffer = buffer;
        m_map.clear();
    }
private:
    template<boost::uint_least16_t id> const char* findFldStart()const {
        right_const_iterator pos = m_map.right.find(fld_consts::FldIdValue<id>::id_espd);
        return (pos != m_map.right.end()) ? m_buffer + pos->second : NULL;
    }

private:
    const char*     m_buffer;
    BiMapT          m_map;
};


typedef void (*FixAction)(const char*, FixMsgWr* );

struct FieldActionMap : private boost::noncopyable {

    typedef  MsgFromTSParser::FieldPosT FieldPosT;
    typedef FieldPosT::const_iterator FldIterT;

    typedef boost::tuple< boost::uint_least16_t,  boost::uint_least16_t, FixAction> FixActionT;

    typedef std::vector<FixActionT> FixActionsT;
    typedef FixActionsT::iterator ActIterT;

    typedef std::pair<FldIterT, ActIterT> FieldActIterT;
    static void dummyAction(const char*, FixMsgWr* ){}

     FieldActionMap(const FieldPosT &fields):
         m_fields(fields) {
         m_actions.reserve(m_fields.capacity());
      }

     FieldActIterT begin(){

         FldIterT f_iter =  m_fields.begin();
         ActIterT a_iter = m_actions.begin();
         return getFieldAct(f_iter, a_iter);
     }

     FieldActIterT next(FieldActIterT iter){
         ++(iter.first);
         ++(iter.second);

         if (  iter.first == m_fields.end()  ){
                return FieldActIterT(iter.first, iter.second);
          }
         return getFieldAct(iter.first, iter.second) ;
     }

     bool isEnd(FieldActIterT iter ) const {
         return (iter.first == m_fields.end() );
     }
     void setAction(  FieldActIterT &iter, FixAction act){
        (iter.second)->get<2>() = act;
     }

     FixAction  getAction(  FieldActIterT &iter) const{ return (iter.second)->get<2>(); }
     const char* getPos(FieldActIterT &iter) const { return *(iter.first); }
     boost::uint_least16_t getBigId(FieldActIterT &iter) const { return (iter.second)->get<0>(); }
     boost::uint_least16_t getId(FieldActIterT &iter) const { return (iter.second)->get<1>(); }

private:
     FieldActIterT getFieldAct(FldIterT f_iter,  ActIterT a_iter){
         if(f_iter == m_fields.end() ) { // should not happened TODO Diag
            return FieldActIterT(f_iter, a_iter );
         }
         const char *fld = *f_iter;
         if(!fld ) { // should not happened TODO Diag
             return FieldActIterT(f_iter, a_iter );
         }
         const FldId *big_id = (const FldId  *)fld;
         const boost::uint_least16_t ubig_id = big_id->get_espd();
         if(a_iter == m_actions.end()){// first time
             FixActionsT::value_type act(ubig_id, big_id->get(), NULL );
             m_actions.push_back(act);
         }else if( a_iter->get<0>()  != ubig_id ){// Order has been changed !!!
             a_iter->get<0>() = ubig_id;
             a_iter->get<1>() = big_id->get();
             a_iter->get<2>() = NULL;
         }
         return (FieldActIterT(f_iter, a_iter));
     }

private:
    const FieldPosT         &m_fields;
    FixActionsT             m_actions;
};

typedef boost::shared_ptr<FieldActionMap> FieldActionMapPtr;

//TODO make return false if not OK
template<boost::uint_least16_t id, typename FT> struct Act;

template< typename FT> struct Act<CFETP_FIELD_TS_ID, FT>{

    static void make( const char* fld, FixMsgWr* msg){
        if(!msg) return ;
        boost::uint_least32_t tsId  =  MsgField< IdType< CFETP_FIELD_TS_ID >:: type > ::get_data(fld);
        msg->set_tsId(tsId);
    }
};

template< typename FT> struct Act<CFETP_FIELD_EXT_TS_SESSIONID, FT>{

    static void make( const char* fld, FixMsgWr* msg){
        if(!msg) return ;
        boost::uint_least32_t xsessId = MsgField< IdType< CFETP_FIELD_EXT_TS_SESSIONID >:: type > ::get_data(fld);
        msg->set_extSesId(xsessId);
    }
};


template< typename FT> struct Act<CFETP_FIELD_CLIENT_ORDER_ID, FT>{

    static void make( const char* fld, FixMsgWr* msg){
        if(!msg) return ;
        const std::string  clOrdID =  MsgField< MsgFieldT< CFETI_FIELDTYPE_STRING > >::get_data(fld);
        FIX::Message &fix_msg =  msg->getMsg();
        fix_msg.setField(FIX::OrigClOrdID(clOrdID ));
    }
};
///TODO the same as CFETP_FIELD_CLIENT_ORDER_ID ???
template< typename FT> struct Act<CFETP_FIELD_ORIG_CLIENT_ORDER_ID, FT>{

    static void make( const char* fld, FixMsgWr* msg){
        if(!msg) return ;
        const std::string  origClOrdID  =  MsgField< MsgFieldT< CFETI_FIELDTYPE_STRING > >::get_data(fld);
        FIX::Message &fix_msg =  msg->getMsg();
        fix_msg.setField(FIX::OrigClOrdID(origClOrdID ) );
    }
};

template< typename FT> struct Act<CFETP_FIELD_INSTNAME, FT>{

    static void make( const char* fld, FixMsgWr* msg){
        if(!msg) return ;
        const std::string   symbol =  MsgField< MsgFieldT< CFETI_FIELDTYPE_STRING > >::get_data(fld);
        FIX::Message &fix_msg =  msg->getMsg();
        fix_msg.setField(FIX::Symbol(symbol));
    }
};

template< typename FT> struct Act<CFETP_FIELD_PRICE, FT>{

    static void make( const char* fld, FixMsgWr* msg){
        if(!msg) return ;
        double price =  MsgField< MsgFieldT< CFETI_FIELDTYPE_DECIMAL > >::get_data(fld);
        FIX::Message &fix_msg =  msg->getMsg();
        fix_msg.setField(FIX::Price(price ));
    }
};

template< typename FT> struct Act<CFETP_FIELD_SIZE, FT>{

    static void make( const char* fld, FixMsgWr* msg){
        if(!msg) return ;
        double size =  MsgField< MsgFieldT< CFETI_FIELDTYPE_DECIMAL > >::get_data(fld);
        FIX::Message &fix_msg =  msg->getMsg();
        fix_msg.setField(FIX::OrderQty(size ));
    }
};

template< typename FT> struct Act<CFETP_FIELD_INDICATOR, FT>{

    static void make( const char* fld, FixMsgWr* msg){
        if(!msg) return ;
        boost::uint_least8_t indicator =  MsgField< MsgFieldT< CFETI_FIELDTYPE_BYTE > >::get_data(fld);
        const char side = ESPD_FIX_MAP.getSide(indicator);
        FIX::Message &fix_msg =  msg->getMsg();
        fix_msg.setField(FIX::Side(side) );
    }
};

template< typename FT> struct Act<CFETP_FIELD_ID, FT>{

    static void make( const char* fld, FixMsgWr* msg){
        if(!msg) return ;
        boost::uint_least32_t id =  MsgField< MsgFieldT< CFETI_FIELDTYPE_UINT32 > >::get_data(fld);
        std::string orderId;
        try{
            orderId = boost::lexical_cast<std::string>(id);
        }catch(...){ return ;}
        FIX::Message &fix_msg =  msg->getMsg();
        fix_msg.setField(FIX::OrderID(orderId )  );
    }
};




}//
#endif
