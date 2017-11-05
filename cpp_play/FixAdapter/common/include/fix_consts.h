
#ifndef FIX_CONSTS_H
#define FIX_CONSTS_H
#include "fixI.h"

namespace FIX {

	const char OrdType_MARKET_IF_TOUCHED_LIMIT = 'R';

	const char Side_INVALID = '\0';
	const char OrdType_INLALID = '\0';
	const char TimeInForce_INVALID = '\0';
	const char MsgType_INVALID = '\0';



	namespace FIELD {
		const int INVALID_TAG = -1;


	}// namespace FIELD
}//namespace FIX

namespace other_fix {

	template< int fix_fld> struct FixFldT {
		enum { value = fix_fld };
	};

	template <char ch1, char ch2 = FIX::MsgType_INVALID > struct FixMsgType {
		enum { CH1 = ch1, CH2 = ch2 };
	};
	enum FixProtocol { FIX_42, FIX_44 };

	typedef FixMsgType< 'D' > FixMsgNewOrderSingle;
	typedef FixMsgType< 'G' > FixMsgOrderCancelReplaceRequest;
	typedef FixMsgType< 'A' > FixMsgLogon;
	typedef FixMsgType< '8' > FixMsgExecutionReport;

	template < FixProtocol p, typename Type > struct FixMsgProtocolType {
		enum { protocol = p };
		typedef Type fix_type;
	};

}//namespace other_fix

#endif
