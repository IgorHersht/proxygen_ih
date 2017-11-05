
#include "ts_response_manager.h"
#include "msh_field_type.h"
#include "msg_buffer_map.h"
#include "session_manager.h"

namespace other_fix {

	void TSResponseManager::init(const SessionsInfo &config) {
		try {

			m_thread = ThreadEntryPtr(new ThreadEntryT(this));
			const TradingSystemId id = m_router.getTradingSystemId();
			if (consts::INVALID_TS_ID == id) {
				throw ESPDError("Invalid trading system id.");
			}
			ConverterSrcTargetID src = tsId2CoverterType(id);
			if (src == UNKNOWN_CONVERTER_ST_ID) {
				throw ESPDError("Invalid converter source id.");
			}
			m_converterSrcId = src;

		}
		catch (...) {
			errorxxx("Cannot init  TSResponseManager ");
			throw;
		}
	}

	void TSResponseManager::manageEspdMsg() {
		try {
			BufferPtr buff(NULL);
			m_router.consume(buff);// pop from the ts queue
			if (!buff) {
				errorxxx("NULL Buffer. Message is ignored.");
				return;
			}

			FixMessageInfo fixMsg(NULL, NULL);
			manageEspdMsg(buff, fixMsg);
			m_router.delocate(buff);
			if (fixMsg.first && fixMsg.second && (*fixMsg.second != fix_consts::INVALID_SESSION_ID)) {
				m_sessionManager->fixMsgFromTS(*fixMsg.first, *fixMsg.second);
			}
			return;
		}
		catch (ESPDMsgError &e) {
			errorxxx(std::string(e.what()) + " Message is ignored.");
			return;
		}
		catch (...) {
			errorxxx("Unkown error. Message is ignored.");
			return;
		}
	}

	void TSResponseManagers::init(const SettingsWrapper &settings, RoutersT &routers) {

		try {
			SessionsInfoPtr sessionInfo = settings.getSessionsInfo();
			if (!sessionInfo) {
				warningxxx("Cannot find session Info.");
				return;
			}
			const TradingSystemsId& ids = settings.getTsids();
			for (TradingSystemsId::const_iterator pos = ids.begin(), end = ids.end(); pos != end; ++pos) {

				RouterT::FixTsRouterPtr router = routers.find(*pos);
				if (!router) {
					errorxxx("Cannot find router.");
					continue;
				}
				TSResponseManagerPtr ptr(new TSResponseManager(*sessionInfo, *router));
				m_managers.insert(TSResponseManagersT::value_type(*pos, ptr));
			}
		}
		catch (...) {
			errorxxx("Cannot init  TSResponseManagers ");
		}
	}

}
