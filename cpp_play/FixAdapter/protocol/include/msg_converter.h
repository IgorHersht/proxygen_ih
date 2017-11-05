#ifndef MSG_CONVERTER_H
#define MSG_CONVERTER_H


#include <boost/static_assert.hpp>
#include "logger.h"
#include "utils.h"
#include "fix_consts.h"
#include "msg_converter_I.h"

namespace other_fix {


	template<ConverterSrcTargetID src, ConverterSrcTargetID target  >
	struct ConverterSrcImpl : virtual public MsgConverterBaseWr< src, target >::base_type {
		typedef typename  MsgConverterBaseWr< src, target >::base_type     BaseT;

		template <typename ArgT>
		explicit  ConverterSrcImpl(const ArgT &arg) : BaseT(arg) {
		}
		template <typename SrcSubT, typename TargetSubT>
		void convert(const SrcSubT &sSrc, TargetSubT &sTarget) {

		}
	};

	template<ConverterSrcTargetID src, ConverterSrcTargetID target  >
	struct ConverterTargetImpl : virtual public MsgConverterBaseWr< src, target >::base_type {
		typedef typename  MsgConverterBaseWr< src, target >::base_type     BaseT;
		template <typename ArgT>
		explicit  ConverterTargetImpl(const ArgT &arg) : BaseT(arg) {
		}
		template <typename SrcSubT, typename TargetSubT>
		void convert(const SrcSubT &sSrc, TargetSubT &sTarget) {

		}
	};

	template<ConverterSrcTargetID src, ConverterSrcTargetID target  >
	struct ConverterImpl : public ConverterSrcImpl<src, target>, public ConverterTargetImpl<src, target> {

		typedef ConverterSrcImpl< src, target >         SrcT;
		typedef ConverterTargetImpl< src, target >      TargetT;
		BOOST_STATIC_ASSERT((boost::is_same< typename  SrcT::BaseT, typename  TargetT::BaseT>::value));
		typedef typename  SrcT::BaseT                   BaseT;

		template <typename ArgT>
		explicit  ConverterImpl(const ArgT &arg) : BaseT(arg), SrcT(arg), TargetT(arg) {

		}
		template <typename SrcSubT, typename TargetSubT>
		void convert(const SrcSubT &sSrc, TargetSubT &sTarget) {
			BaseT::convert(sSrc, sTarget);
			SrcT::convert(sSrc, sTarget);
			TargetT::convert(sSrc, sTarget);
		}
	};


	template<ConverterSrcTargetID src, ConverterSrcTargetID target>
	struct MsgConverter : public ConverterImpl< src, target > {
		enum { SRC = src, TARGET = target };
		typedef ConverterImpl< src, target >       BaseT;

		typedef typename BaseT::SrcT               SrcT;
		typedef typename BaseT::TargetT            TargetT;
		typedef typename BaseT::BaseT              VBaseT;

		template <typename ArgT>
		explicit  MsgConverter(const ArgT &arg) :VBaseT(arg), BaseT(arg) {
		}

		template <typename SrcSubT, typename TargetSubT>
		void convert(const SrcSubT &sSrc, TargetSubT &sTarget) {
			BaseT::convert(sSrc, sTarget);
		}
		template <typename SrcSubT, typename TargetSubT>
		void convertSrc(const SrcSubT &sSrc, TargetSubT &sTarget) {
			SrcT::convert(sSrc, sTarget);
		}
		template <typename SrcSubT, typename TargetSubT>
		void convertTarget(const SrcSubT &sSrc, TargetSubT &sTarget) {
			TargetT::convert(sSrc, sTarget);
		}
		template <typename SrcSubT, typename TargetSubT>
		void convertBase(const SrcSubT &sSrc, TargetSubT &sTarget) {
			VBaseT::convert(sSrc, sTarget);
		}
	};


	class  MsgConverterFactory : private boost::noncopyable {
		typedef boost::shared_ptr<void> VoidPtr;
	public:
		MsgConverterFactory() :
			m_converters(NUM_CONVERTER_ST_ID, std::vector< VoidPtr>(NUM_CONVERTER_ST_ID, VoidPtr())) {
		}

		template <ConverterSrcTargetID src, ConverterSrcTargetID target>
		MsgConverter< src, target>*  getMsgConverter(const typename MsgConverterBaseWr<src, target>::base_type::ArgT &arg) {
			typedef MsgConverter< src, target> ConverterT;
			VoidPtr   &ptr = (m_converters.at(src)).at(target);
			if (!ptr) {
				ptr = boost::shared_ptr<ConverterT>(new ConverterT(arg));
			}

			return ((ConverterT*)ptr.get());
		}

	private:
		std::vector<std::vector< VoidPtr> >  m_converters;
	};



}//
#endif
