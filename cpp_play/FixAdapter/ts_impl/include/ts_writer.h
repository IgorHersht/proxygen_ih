#ifndef _TS_WRITER_H
#define _TS_WRITER_H

#include "ts_connector.h"
#include "ts_settings.h"
#include "ts_rw_base.h"
#include "logger.h"

namespace other_fix {


	struct TsWriterImpl {
		typedef FixToTsRouter RouterT;
		const static std::string IMPL_TYPE;
		enum { INIT_BUFFER_NUM = 128 };


		TsWriterImpl(const TradingSytemSettings &settings, TsConnector &connector, RouterT &router) :
			m_settings(settings),
			m_connector(connector),
			m_router(router),
			m_buffers(INIT_BUFFER_NUM) {
		}

		void threadFunction() {

			bool status = consume();
			if (!status) {
				return;
			}
			timing::LatencyItems::getInstance().setTime("Sending espd msg to ts;  m_connector.send_to_ts");
			m_connector.send_to_ts(m_buffers.getConnectorBuffers());
			delocate();
		}
	protected:

		bool consume() {
			bool status = consumeNonBloking(); // try non-bloking
			if (!status) {
				status = consumeBloking(); // try bloking
			}
			return status;
		}

		bool consumeNonBloking() {
			BufferPtr buff(NULL);
			while (m_router.tryConsume(buff)) {
				if (!buff) {
					warningxxx("Empty buffer on the queue");
					continue;
				}
				m_buffers.push_back(buff);
			}
			return !m_buffers.empty();
		}

		bool consumeBloking() {
			BufferPtr buff(NULL);
			m_router.consume(buff);
			if (!buff) {
				logxxx("Empty buffer on  the queue");
				return false;
			}
			m_buffers.push_back(buff);
			return true;
		}

		void delocate() {
			m_buffers.clear(m_router);
		}
	protected:
		const TradingSytemSettings                                  &m_settings;
		TsConnector                                                 &m_connector;
		RouterT                                                     &m_router;
		BufferSequenceAdapter<RouterT, TsConnector::BuffersT>       m_buffers;
	};

	typedef TsRW<TsWriterImpl> TsWriter;
	typedef boost::shared_ptr<TsWriter> TsWriterPtr;
	typedef TsRsWs<TsWriterImpl> TsWriters;
	typedef boost::shared_ptr<TsWriters> TsWritersPtr;

}  //namespace other_fix

#endif
