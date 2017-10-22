/*
MIT License

Copyright @ 2017 Igor Hersht

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
#include <stdio.h>
#include <string>
#include <exception>
#include <string>
#include <map>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include "Logger.h"

namespace cpp_tools {

const std::string EmptyStr;
template<typename T, std::size_t size> inline std::size_t ArraySize(
		T (&)[size]) {
	return size;
}

template<size_t size> inline char* str2Cstr(char (&ar)[size],
		const std::string &from) {
	// static_assert(size > 0, "cannot copy to zero array");
	if (from.empty()) {
		return NULL;
	}
	char *dest = strncpy(ar, from.c_str(), size - 1);
	ar[size - 1] = '\0';
	return dest;
}

typedef std::map<std::string, std::string> OptionsT;
typedef boost::shared_ptr<OptionsT> OptionsPtr;

typedef boost::shared_ptr<FILE> FilePtr;

inline void FileCloser(FILE *file) {
	if (file) {
		try {
			fclose(file);
		} catch (...) {
		}
	}
}

inline FilePtr OpenFile(const std::string &fileName, const std::string &mode) {

	FilePtr ptr;
	try {
		ptr = FilePtr(fopen(fileName.c_str(), mode.c_str()), FileCloser);
	} catch (...) {
	}
	return ptr;
}

inline size_t MakeSafeStdString(const char* cStr, size_t maxSize,
		std::string & stdStr) {
	if (!cStr) {
		return 0;
	}
	size_t size = strnlen(cStr, maxSize);
	if (size == maxSize) {
		return 0;
	}
	stdStr = std::string(cStr, size);
	return size;
}

inline bool setWD(const std::string &dir) {
	namespace fs = boost::filesystem;
	if (dir.empty()) {
		return false;
	}
	try {
		fs::path oldWD = fs::current_path();
		fs::path newWD(dir);
		if ((!fs::exists(newWD)) || (!fs::is_directory(newWD))) {
			return false;
		}
		if (fs::equivalent(oldWD, newWD)) {
			return true;
		}
		fs::current_path(newWD);
		return true;

	} catch (...) {
		return false;
	}
}

template<class T>
typename boost::enable_if<boost::is_convertible<T, std::string>, std::string>::type ValueToString(
		const T &v) {
	return v;
}

template<typename T> typename boost::disable_if<
		boost::is_convertible<T, std::string>, std::string>::type ValueToString(
		const T &v) {
	try {
		return boost::lexical_cast < std::string > (v);
	} catch (...) {
		return std::string();
	}
}

template<class T> std::string ValueToString(const T *ptr) {
	if (!ptr) {
		return std::string();
	}
	return ValueToString(*ptr);
}

template<class T> std::string ValueToString(boost::shared_ptr<T> ptr) {
	if (!ptr) {
		return std::string();
	}
	return ValueToString(*ptr);
}

template<typename T1, typename T2> inline std::string ValueToString(
		const std::pair<T1, T2> &v) {
	try {
		return (ValueToString(v.first) + "," + ValueToString(v.second));
	} catch (...) {
		return ",";
	}
}

template<typename T> inline std::string VarToString(std::string var,
		const T &v) {
	return (var + "=" + ValueToString(v) + ";\n");
}

template<typename T> inline std::string ColToString(std::string var,
		const T &col) {
	std::string value = var + "=";
	for (typename T::const_iterator pos = col.begin(), end = col.end();
			pos != end; ++pos) {
		value += ValueToString(*pos);
		value += ";";
	}
	value += "\n";
	return value;
}

}

