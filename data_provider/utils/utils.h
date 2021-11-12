#pragma once

#include<vector>
#include <boost/asio.hpp>

namespace network {
    class MaxAlignedBuffer;
    typedef std::shared_ptr<MaxAlignedBuffer> BufferPtr;

struct MaxAlignedBuffer
{
    typedef long double Scalar;
    static_assert( alignof(std::max_align_t) == alignof(std::max_align_t), "bad aligment");
    MaxAlignedBuffer() = default;
    void setSize(size_t size){ //  assumes size > 0
        _size = size;
        _buffer.resize((_size/sizeof(Scalar)) +1); // alocate enough.
    }
    void setBuffer(const void* src, size_t size){
        setSize(size);
        memcpy(getPtr<void>(), src, size);
    }
    boost::asio::mutable_buffer  getAsioBuffer() const{
        return boost::asio::buffer( getPtr<void>(), _size );
    }
    template<typename T> T* getPtr() const{
        return (T*)(&(_buffer.at(0)));
    }
    size_t getSize()const{
        return _size;
    }
    friend MaxAlignedBuffer operator+(const MaxAlignedBuffer l, const MaxAlignedBuffer r  );
    friend  BufferPtr operator+(const BufferPtr l, const BufferPtr r  );

private:
    std::vector<long double> _buffer;
    size_t _size{0};
};

inline   MaxAlignedBuffer operator+(const MaxAlignedBuffer l, const MaxAlignedBuffer r  )
{
    MaxAlignedBuffer out;
    out.setSize(l._size + r._size);
    memcpy(out.getPtr<char>(), l.getPtr<char>(),l._size );
    memcpy(out.getPtr<char>() + l._size , r.getPtr<char>(),r._size );
    return out;
}

inline BufferPtr operator+(const BufferPtr l, const BufferPtr r  )
{
    BufferPtr out = std::make_shared<MaxAlignedBuffer>();
    out->setSize(l->_size + r->_size);
    memcpy(out->getPtr<char>(), l->getPtr<char>(),l->_size );
    memcpy(out->getPtr<char>() + l->_size , r->getPtr<char>(),r->_size );
    return out;

}

}

