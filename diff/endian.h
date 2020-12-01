#pragma once
#include <boost/endian/conversion.hpp>

template <typename T> inline T native_to_big(T value){
	return boost::endian::native_to_big(value);
}

inline float native_to_big(float value){
	uint32_t storage = *(reinterpret_cast<uint32_t*>(&value));
	storage = boost::endian::native_to_big(storage);
	value = *(reinterpret_cast<float* >(&storage));
	return value;

}
inline double native_to_big(double value){
	uint64_t storage = *(reinterpret_cast<uint64_t*>(&value));
	storage = boost::endian::native_to_big(storage);
	value = *(reinterpret_cast<double* >(&storage));
	return value;
}

template <typename T> inline T big_to_native(T value){
	return boost::endian::big_to_native(value);
}

inline float big_to_native(float value){
	uint32_t storage = *(reinterpret_cast<uint32_t*>(&value));
	storage = boost::endian::big_to_native(storage);
	value = *(reinterpret_cast<float* >(&storage));
	return value;

}
inline double big_to_native(double value){
	uint64_t storage = *(reinterpret_cast<uint64_t*>(&value));
	storage = boost::endian::big_to_native(storage);
	value = *(reinterpret_cast<double* >(&storage));
	return value;
}




