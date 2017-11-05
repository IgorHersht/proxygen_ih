#ifndef _TS_READER_H
#define _TS_READER_H

#include "ts_connector.h"
#include "ts_settings.h"
#include "ts_rw_base.h"  
#include "msg_header.h"


namespace other_fix {


	struct TsReaderImpl {
		const static std::string IMPL_TYPE;
		typedef TSToFixRouter RouterT;
		TsReaderImpl(const TradingSytemSettings &settings, TsConnector &connector, RouterT &router) :
			m_settings(settings), m_connector(connector), m_router(router) {
		}

		void threadFunction() {

			BufferPtr buff(NULL);
			bool status = m_router.allocate(buff);
			if ((!buff) || (!status)) {
				errorxxx("Cannot allocate buffer for a responce. ");
				return;
			}
			loadMsg(buff);
			timing::LatencyItems::getInstance().setTime("Reseved msg from ts;  loadMsg(buff)");
			m_router.produce(buff);
		}
	private:
		void loadMsg(BufferPtr buff) {
			m_connector.receive_from_ts(TsConnector::Buffer_1T(buff->get_buf(), sizeof(MsgHeader)));
			MsgHeader *hdr = (MsgHeader *)buff->get_buf();
			const boost::uint_least16_t bodySize = hdr->get_length();
			m_connector.receive_from_ts(TsConnector::Buffer_1T(buff->get_buf() + sizeof(MsgHeader), bodySize));
			buff->set_offset(sizeof(MsgHeader) + bodySize);
		}

	protected:
		const TradingSytemSettings               &m_settings;
		TsConnector                              &m_connector;
		RouterT                                  &m_router;
	};

	typedef TsRW<TsReaderImpl> TsReader;
	typedef boost::shared_ptr<TsReader> TsReaderPtr;
	typedef TsRsWs<TsReaderImpl> TsReaders;
	typedef boost::shared_ptr<TsReaders> TsReadersPtr;


}
#endif
