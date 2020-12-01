/*
 *
 *  Created on: Sep 24, 2014
 *      Author: ihersht
 */

//#pragma once

#include <string>
#include <boost/thread.hpp>
#include <boost/current_function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <sys/types.h>
#include <unistd.h>
namespace cpp_tools{

namespace _private{

static boost::mutex cout_mutex;
 inline void write_to_std_out( const std::string &msg){
	boost::mutex::scoped_lock lock(cout_mutex);
	try{
		std::cout <<msg<<std::endl<< std::flush;
	}catch(...){

	}
}
}

const int DBG = -1;
const int INF =  0;
const int WRN = 1;
const int ERR = 2;

 inline void default_log( std::string msg, int sev ){
 	std:: string prefix;
 	switch(sev){
 		case DBG: {
 	 			prefix = " DBG-> ";
 	 			break;
 	 	}
 		case INF: {
 			prefix = " INF-> ";
 			break;
 		}
 		case WRN: {
 			prefix = " WARN-> ";
 			break;
 		}
 		case ERR: {
 			prefix = " ERR-> ";
 			break;
 		}
	};
//#ifdef DEBUG
	_private::write_to_std_out(prefix + msg);
//#else
	if(sev == ERR){
		_private::write_to_std_out(prefix + msg);
	}
//#endif

}

typedef boost::function<void (std::string,int )> logger_type;


 inline void log(  std::string msg, int sev, logger_type loggerInit = default_log ){

	 static logger_type logger = loggerInit;
	 logger(msg, sev);

 }


const std::string LOCATION_SEP(":");
#define SRC_LOCATION ( std::string(__FILE__) + cpp_tools::LOCATION_SEP  + \
 		boost::lexical_cast<std::string>(__LINE__)  + cpp_tools::LOCATION_SEP + \
 		std::string(__func__) + cpp_tools::LOCATION_SEP )
 		///std::string(BOOST_CURRENT_FUNCTION) + cpp_tools::LOCATION_SEP )

inline std::string MakeLogFileName(const std::string &prefix){
	const std::string suffix = ".log";
	const std::string sep = "_";
	std::string timeStr = boost::posix_time::to_iso_string(boost::posix_time::microsec_clock::local_time());
	std::string pidStr= boost::lexical_cast<std::string>(getpid());

	std::string name = prefix; name += sep;
	name += timeStr; name += sep;
	name += pidStr; name += suffix;

	return name;
}

 }

////////////
/*
 * Options.h
 *
 *  Created on: Sep 4, 2015
 *      Author: ihersht
 */

//#pragma once

#include <string>
#include <map>
#include <boost/lexical_cast.hpp>
#include<boost/array.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/numeric/conversion/cast.hpp>



namespace cpp_tools{

template <int pos> struct PosT{
 enum{m_pos = pos };

};

template< int Size,  template <int> class  OptionTraitT> class Options{

typedef std::pair<std::string, std::string>				KeyValuel;
typedef boost::array< KeyValuel,  (std::size_t)Size>	KeyValueMap;
BOOST_STATIC_ASSERT_MSG(  Size > 0 , "Wrong size.");

public:
typedef boost::function<std::string (std::string)> LoaderT;
Options(logger_type	logger, LoaderT loader):m_logger(logger), m_loader(loader){

}

bool init(){
	bool status = init(PosT< Size  -1>());
	return status;
}

bool loadValues(){
	bool status = loadValues( PosT< Size  -1>());
	return status;
}

void dump() const{
	std::string msg = SRC_LOCATION + "Thrift options dump\n" ;
	for(int pos =0; pos < KeyValueMap::static_size; ++pos){
		try{
			KeyValuel kv = m_keyValueMap.at(pos);
			msg += kv.first; msg +="->"; msg += kv.second;
			msg +="\n";
		}catch(...){
			std::string msg = SRC_LOCATION + "Coding error. Invalid option" ;
			cpp_tools::log(msg ,cpp_tools::ERR);
		}
	}
	cpp_tools::log(msg ,cpp_tools::INF);
}

template <int pos>  typename OptionTraitT<pos>::value_type getOption(PosT<pos>){
	return getOption<pos>();
}

template <int pos>  typename OptionTraitT<pos>::value_type getOption(){
	BOOST_STATIC_ASSERT_MSG(  (pos >= 0) && (pos < KeyValueMap::static_size) , "Wrong position.");
	typedef typename OptionTraitT<pos >::value_type value_type;

	value_type value = OptionTraitT<pos >::default_value();
	try{
	   value = boost::lexical_cast<value_type>(m_keyValueMap.at(pos).second);
	}catch(...){
		std::string msg = SRC_LOCATION + "Coding error. Invalid option" ;
		cpp_tools::log(msg ,cpp_tools::ERR);
	}
	return value;
}


private:

template <int pos> bool init(PosT<pos> ){
	BOOST_STATIC_ASSERT_MSG(  (pos >= 0) && (pos < KeyValueMap::static_size) , "Wrong position.");
	try{
		const std::string str_key = OptionTraitT<pos >::string_key();
		const std::string str_value =  boost::lexical_cast<std::string>(OptionTraitT<pos>::default_value());
		m_keyValueMap.at(pos) = KeyValuel(str_key, str_value);
	}catch(...){
		std::string msg = SRC_LOCATION + "Coding error. Invalid option" ;
		cpp_tools::log(msg ,cpp_tools::ERR);
		return false;
	}
	return init(PosT<pos -1>());
}

bool init(PosT< -1> ){/// stop recirsion  on -1
	return true;
}

template <int pos> bool loadValues( PosT<pos> pt){
	BOOST_STATIC_ASSERT_MSG(  (pos >= 0) && (pos < KeyValueMap::static_size) , "Wrong position.");
	try{
		const std::string str_key = OptionTraitT<pos >::string_key();
		const std::string str_value =  m_loader(str_key);
		if(!(str_value.empty())){
			bool isValid = isValueValid(pt, str_value);
			if(!isValid ){
				std::string msg = SRC_LOCATION + "Invalid key->value pair: " ;
				msg += str_key; msg += "->"; msg += str_value;  msg += ".";
				cpp_tools::log(msg ,cpp_tools::ERR);
				return false;
			}
			m_keyValueMap.at(pos) = KeyValuel(str_key, str_value);
		}
	}catch(...){
		std::string msg = SRC_LOCATION + "Coding error. Invalid option" ;
		cpp_tools::log(msg ,cpp_tools::ERR);
		return false;
	}

	return loadValues(PosT<pos -1>());
}


bool loadValues( PosT<-1> ){
	return true;
}

template <int pos> bool isValueValid( PosT<pos>, const std::string &strValue ){

	typename OptionTraitT<pos >::value_type value = OptionTraitT<pos >::default_value();
	if(!boost::conversion::try_lexical_convert(strValue, value)){
		return false;
	}
	return true;
}

private:
logger_type			m_logger;
LoaderT 			m_loader;
KeyValueMap			m_keyValueMap;

};


}

enum OptionKey{
	////Zookeeper and LoadBalancer
	ZOOKEEPER_SERVER,
	THRIFT_CLIENTS_NUMBER,
	THRIFT_LINGER_ON,
	_OptionKeyNum_
};

template<  int key> struct OptionTrait;


template <> struct OptionTrait< ZOOKEEPER_SERVER>{
	static std::string string_key(){return "ZOOKEEPER_SERVER";}
	typedef std::string value_type;
	static value_type default_value(){ return "server-zookeeper.inw-arp-dev.rfiserve.net:2181" ;}
};
template <> struct OptionTrait<  THRIFT_CLIENTS_NUMBER>{
	static std::string string_key(){return "THRIFT_CLIENTS_NUMBER";}
	typedef int value_type;
	static value_type default_value(){ return 15 ;}
};
template <> struct OptionTrait<  THRIFT_LINGER_ON>{
	static std::string string_key(){return "THRIFT_LINGER_ON";}
	typedef bool value_type;
	static value_type default_value(){ return true ;}
};

std::string loader(std::string key){
	if( key == "ZOOKEEPER_SERVER") return "Server";
	if( key == "THRIFT_CLIENTS_NUMBER") return "16";
	if( key == "THRIFT_LINGER_ON") return "0";
	return 		"";
}



void testOptions(){

	cpp_tools::Options<  _OptionKeyNum_,  OptionTrait > options(cpp_tools::default_log, loader);
	bool status =  options.init();
	status =  options.loadValues();
	options.dump();
	OptionTrait< ZOOKEEPER_SERVER>::value_type zs = options.getOption(cpp_tools::PosT<ZOOKEEPER_SERVER>() );
	OptionTrait< THRIFT_CLIENTS_NUMBER>::value_type tn = options.getOption< THRIFT_CLIENTS_NUMBER>();
	OptionTrait< THRIFT_LINGER_ON >::value_type tl = options.getOption< THRIFT_LINGER_ON>();

	int i =0;
}



int main() {
	testOptions();
	return 0;
}

