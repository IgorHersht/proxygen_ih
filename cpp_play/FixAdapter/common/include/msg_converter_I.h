#ifndef MSG_CONVERTER_I_H
#define MSG_CONVERTER_I_H

namespace FIX{
    class SessionID;
}

namespace other_fix {

struct MsgToTs;
struct MsgFromTs;
struct TSRequestsManager;
struct TSResponseManager;

enum ConverterSrcTargetID     { 
    UNKNOWN_CONVERTER_ST_ID = -1, 
    FIX_4_2_ST, 
    FIX_4_4_ST, 
    UST_ID_ST, 
    ELX_ID_ST,
    FXO_ID_ST,
    ANY_TS_ST,
    NUM_CONVERTER_ST_ID  
};
template<ConverterSrcTargetID src, ConverterSrcTargetID target >struct MsgConverterBaseWr;

template< >struct MsgConverterBaseWr < FIX_4_2_ST , UST_ID_ST  >{
    typedef MsgToTs              base_type;
    typedef TSRequestsManager    manager_type;
    typedef FIX::SessionID       session_id_type;
};
template<  >struct MsgConverterBaseWr < FIX_4_2_ST , ELX_ID_ST >{
    typedef MsgToTs             base_type;
    typedef TSRequestsManager    manager_type;
    typedef FIX::SessionID      session_id_type;
};
template<  >struct MsgConverterBaseWr < FIX_4_2_ST , FXO_ID_ST >{
    typedef MsgToTs             base_type;
    typedef TSRequestsManager    manager_type;
    typedef FIX::SessionID      session_id_type;
};
template<  >struct MsgConverterBaseWr < FIX_4_2_ST , ANY_TS_ST >{
    typedef MsgToTs             base_type;
    typedef TSRequestsManager    manager_type;
    typedef FIX::SessionID      session_id_type;
};
template< >struct MsgConverterBaseWr < FIX_4_4_ST , UST_ID_ST >{
     typedef MsgToTs                base_type;
     typedef TSRequestsManager      manager_type;
     typedef FIX::SessionID         session_id_type;
};
template<  >struct MsgConverterBaseWr<  FIX_4_4_ST , ELX_ID_ST  >{
    typedef MsgToTs             base_type;
    typedef TSRequestsManager    manager_type;
    typedef FIX::SessionID      session_id_type;
};
template<  >struct MsgConverterBaseWr < FIX_4_4_ST , FXO_ID_ST >{
    typedef MsgToTs             base_type;
    typedef TSRequestsManager    manager_type;
    typedef FIX::SessionID      session_id_type;
};
template<  >struct MsgConverterBaseWr < FIX_4_4_ST , ANY_TS_ST>{
    typedef MsgToTs             base_type;
    typedef TSRequestsManager    manager_type;
    typedef FIX::SessionID      session_id_type;
};
template<   >struct MsgConverterBaseWr <  UST_ID_ST, FIX_4_2_ST  >{
    typedef MsgFromTs               base_type;
    typedef TSResponseManager      manager_type;
    typedef FIX::SessionID         session_id_type;
};
template<  >struct MsgConverterBaseWr <  ELX_ID_ST, FIX_4_2_ST  >{
   typedef MsgFromTs  base_type;
    typedef TSResponseManager      manager_type;
    typedef FIX::SessionID         session_id_type;
};
template<  >struct MsgConverterBaseWr <  FXO_ID_ST, FIX_4_2_ST  >{
   typedef MsgFromTs  base_type;
    typedef TSResponseManager      manager_type;
    typedef FIX::SessionID         session_id_type;
};
template<  >struct MsgConverterBaseWr <  ANY_TS_ST, FIX_4_2_ST  >{
   typedef MsgFromTs  base_type;
    typedef TSResponseManager      manager_type;
    typedef FIX::SessionID         session_id_type;
};
 template<  >struct MsgConverterBaseWr <  UST_ID_ST, FIX_4_4_ST  >{
    typedef MsgFromTs  base_type;
    typedef TSResponseManager      manager_type;
    typedef FIX::SessionID         session_id_type;
};
template<  >struct MsgConverterBaseWr <  ELX_ID_ST, FIX_4_4_ST  >{
   typedef MsgFromTs              base_type;
   typedef TSResponseManager      manager_type;
   typedef FIX::SessionID         session_id_type;
};
template<  >struct MsgConverterBaseWr <  FXO_ID_ST, FIX_4_4_ST  >{
   typedef MsgFromTs              base_type;
   typedef TSResponseManager      manager_type;
   typedef FIX::SessionID         session_id_type;
};
template<  >struct MsgConverterBaseWr <  ANY_TS_ST, FIX_4_4_ST  >{
   typedef MsgFromTs              base_type;
   typedef TSResponseManager      manager_type;
   typedef FIX::SessionID         session_id_type;
};

} // namespace other_fix

#endif
