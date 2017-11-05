#ifndef  MSG_ENCODER_H
#define  MSG_ENCODER_H

#include <string>
#include <boost/array.hpp>
#include <boost/cast.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>

#include "buffer.h"
#include "msg_header.h"
#include "msh_field_type.h"
#include "logger.h"
#include "fix_espd_maper.h"


namespace other_fix {

	namespace msg_to_ts_ns {
		struct  FieldEncoder : private boost::noncopyable {

			FieldEncoder() {}
			void   reset(BufferPtr buf) {
				m_buf = buf;
				m_buf->reset();
			}
			void  reset() {
				m_buf->reset();
				m_buf = NULL;
			}
			BufferPtr getTsBuffer() const { return m_buf; }
			char*  get_buf() const { return m_buf->get_buf(); }
			size_t get_offset() const { return  m_buf->get_offset(); }
			char*  get_pos() const { return get_buf() + m_buf->get_offset(); }
			void set_offset(size_t offset) { m_buf->set_offset(offset); }

			void append(const char* from, size_t size) {
				if (size == 0) return;
				if ((get_offset() + size) > Buffer::MAX_DATA_SIZE) {
					throw FIXError("Cannot append to the buffer. Buffer is too small. ");
				}
				memcpy(get_pos(), from, size);
				m_buf->add_offset(size);
			}

			//ugly overloading, but no copy
			template < typename T>
			void append() {
				testAppend<T>();
				new(get_pos()) T;
				m_buf->add_offset(sizeof(T));
			}

			template < typename T>
			void appendObj(const T &obj) {
				testAppend<T>();
				memcpy(get_pos(), &obj, sizeof(T));
				m_buf->add_offset(sizeof(T));
			}

			template < typename T, typename Arg1T>
			void append(const Arg1T &arg1) {
				testAppend<T>();
				new(get_pos()) T(arg1);
				m_buf->add_offset(sizeof(T));
			}

			template < typename T, typename Arg1T, typename Arg2T >
			void append(const Arg1T &arg1, const Arg2T &arg2) {
				testAppend<T>();
				new(get_pos()) T(arg1, arg2);
				m_buf->add_offset(sizeof(T));
			}

			template < typename T, typename Arg1T, typename Arg2T, typename Arg3T >
			void append(const Arg1T &arg1, const Arg2T &arg2, const Arg3T &arg3) {
				testAppend<T>();
				new(get_pos()) T(arg1, arg2, arg3);
				m_buf->add_offset(sizeof(T));
			}

			template < typename T, typename Arg1T, typename Arg2T, typename Arg3T, typename Arg4T >
			void append(const Arg1T &arg1, const Arg2T &arg2, const Arg3T &arg3, const Arg4T &arg4) {
				testAppend<T>();
				new(get_pos()) T(arg1, arg2, arg3, arg4);
				m_buf->add_offset(sizeof(T));
			}

			template < typename T, typename Arg1T, typename Arg2T, typename Arg3T, typename Arg4T, typename Arg5T >
			void append(const Arg1T &arg1, const Arg2T &arg2, const Arg3T &arg3, const Arg4T &arg4, const Arg5T &arg5) {
				testAppend<T>();
				new(get_pos()) T(arg1, arg2, arg3, arg4, arg5);
				m_buf->add_offset(sizeof(T));
			}

			template < typename T, typename Arg1T, typename Arg2T, typename Arg3T, typename Arg4T, typename Arg5T, typename Arg6T >
			void append(const Arg1T &arg1, const Arg2T &arg2, const Arg3T &arg3, const Arg4T &arg4, const Arg5T &arg5, const Arg6T &arg6) {
				testAppend<T>();
				new(get_pos()) T(arg1, arg2, arg3, arg4, arg5, arg6);
				m_buf->add_offset(sizeof(T));
			}

			template < typename T, typename Arg1T, typename Arg2T, typename Arg3T, typename Arg4T, typename Arg5T, typename Arg6T, typename Arg7T >
			void append(const Arg1T &arg1, const Arg2T &arg2, const Arg3T &arg3, const Arg4T &arg4, const Arg5T &arg5, const Arg6T &arg6, const Arg7T &arg7) {
				testAppend<T>();
				new(get_pos()) T(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
				m_buf->add_offset(sizeof(T));
			}

			void append(const std::string &str) {
				const size_t len = str.length();
				const size_t offset = get_offset() + len + 1;
				if (offset <= Buffer::MAX_DATA_SIZE) {
					memcpy(get_pos(), str.c_str(), len + 1);
					m_buf->set_offset(offset);
					return;
				}
				throw FIXError("Cannot append to the buffer. Buffer is too small. ");
			}

			void append(char *str) {
				char *end = (char *)memchr(str, '\0', Buffer::MAX_DATA_SIZE - get_offset());// safe strlen
				if (!end) {
					throw FIXError("Cannot append to the buffer. Buffer is too small. ");
				}
				const int len = end - str;
				memcpy(get_pos(), str, len + 1);
				m_buf->add_offset(len + 1);
			}


		private:

			template < typename T> void   testAppend() {
				if ((get_offset() + sizeof(T)) > Buffer::MAX_DATA_SIZE) {
					throw FIXError("Cannot append to the buffer. Buffer is too small. ");
				}
			}
		private:
			BufferPtr                           m_buf;
		};

	} //namespace msg_to_ts_ns

	struct MsgEncoder : private boost::noncopyable {
		MsgEncoder() : m_numFields(0)
		{}
		BufferPtr getTsBuffer() const { return m_buffer.getTsBuffer(); }
		char*  get_buf() const { return m_buffer.get_buf(); }
		size_t get_offset() const { return  m_buffer.get_offset(); }
		char*  get_pos() const { return m_buffer.get_pos(); }
		void set_offset(size_t offset) { m_buffer.set_offset(offset); }



		void reset(BufferPtr buf) {
			m_buffer.reset(buf);
			m_numFields = 0;
		}


		template < typename T>
		void appendObj(const T &obj, int fldNum) {
			BOOST_STATIC_ASSERT(boost::alignment_of<T>::value == boost::alignment_of<char>::value);
			m_buffer.appendObj(obj);
			m_numFields += fldNum;
		}

		void appendBuffer(const char* from, size_t size, int fldNum) {
			m_buffer.append(from, size);
			m_numFields += fldNum;
		}

		template < boost::uint_least16_t id> void appendField(typename IdType< id >::type::native_type data,
			typename boost::enable_if<boost::is_arithmetic<  typename IdType< id >::type::native_type > >::type* dummy = 0) {
			typedef typename IdType<id>::type FT;
			BOOST_STATIC_ASSERT(FT::fixed_size == true);
			//m_buffer.append< MsgField<FT> >( FldHdrValue<id> :: value,  data);
			m_buffer.appendObj(fld_consts::FldHdrValue<id> ::value);
			m_buffer.append<typename FT::espd_type>(data);
			m_numFields++;
		}

		template < boost::uint_least16_t id > void appendField(const std::string &data) {
			BOOST_STATIC_ASSERT(IdType<id>::type::value == CFETI_FIELDTYPE_STRING);
			m_buffer.appendObj(fld_consts::FldHdrValue<id, IdType<id>::type::value> ::value);
			m_buffer.append(data);
			m_numFields++;
		}

		template < boost::uint_least16_t id > void appendField(char *data) {
			BOOST_STATIC_ASSERT(IdType<id>::type::value == CFETI_FIELDTYPE_STRING);
			m_buffer.append<uespd16_t>(id);
			m_buffer.append <uespd8_t>(MsgFieldT< IdType<id>::type::value >::value);
			m_buffer.append(data);
			m_numFields++;
		}

		template < boost::uint_least16_t id, typename InT > void appendFieldWithCast(InT idData) {
			typedef typename IdType< id >::type::native_type NT;
			NT data = NoThrowCaster<NT, InT>::cast(idData);
			return appendField<id>(data);
		}

		//append header
		void insertHdr(boost::uint_least32_t sequenceNo, boost::uint_least16_t messageType, boost::uint_least16_t messageFlags) {

			const boost::uint_least16_t offset = get_offset();
			const boost::uint_least16_t dataLen = boost::numeric_cast<boost::uint_least16_t>(offset - sizeof(MsgHeader));
			set_offset(0);
			m_buffer.append< MsgHeader >(sequenceNo, messageType, messageFlags, m_numFields, dataLen);
			set_offset(offset);
		}

	private:
		msg_to_ts_ns::FieldEncoder      m_buffer;
	protected:
		boost::uint_least16_t           m_numFields;

	};




}// namespace other_fix
#endif
