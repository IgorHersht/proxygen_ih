#pragma once
#include  <cstddef>
#include  <cstdint>
#include <memory>
#include  <utils/utils.h>

namespace network {
namespace _internal {
// TODO have in big endian to use from other languages
struct MsgHeader {
    typedef uint32_t size_type;
    typedef uint32_t msg_type;
    MsgHeader() = default;
    MsgHeader(uint32_t msgSize, uint32_t msgType) :
            _msgSize(msgSize), _msgType(msgType) {}

    size_type getBodySize() const {
        return _msgSize;
    }
    msg_type getMsgType() const {
        return _msgType;
    }
private:
    size_type _msgSize{0};
    msg_type _msgType{0};
};

}// namespace _internal

typedef _internal::MsgHeader RequestHeader;
typedef _internal::MsgHeader ResponseHeader;
typedef MaxAlignedBuffer AlignedMutableBuffer;



}//namespace network

