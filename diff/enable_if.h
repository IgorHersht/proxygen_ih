#ifndef _DALUTILS_H 
#define _DALUTILS_H
#include <stdlib.h>
#include <string.h>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/static_assert.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "DalI.h"



namespace DAL_IMPL{
        
struct DalError : public std::runtime_error{
    explicit  DalError(const std::string &what):std::runtime_error(what){}  
}; 

struct RequestError : public DalError{
    explicit  RequestError(const std::string &what):DalError(what){} 

};
struct RouterError : public DalError{
    explicit   RouterError(const std::string &what):DalError(what){} 

}; 

struct ResponseError : public DalError{
    explicit   ResponseError(const std::string &what):DalError(what){} 
}; 

namespace __private 
{
template < typename ToT> ToT inline InvalidFieldValue(ToT &to){ to = ToT(); }

template < typename ToT> inline void fieldCast( const char* from, ToT &to,
    typename boost::enable_if<boost::is_convertible< const char* , ToT > >::type* dummy = 0) {
   try{
        to = ToT(from);
   }catch(...){
        throw;
   }
   
}

template < typename ToT> inline void fieldCast(  const char* from, ToT &to,
    typename boost::enable_if<boost::is_arithmetic<ToT > >::type* dummy = 0)
{ 
   try{
        to = boost::lexical_cast<ToT>(from);
    }catch(boost::bad_lexical_cast& ){
        throw;
    }
}

}// namespace __private 

template < typename ToT> inline bool fieldCast(  const char* from, ToT &to)
{
   try{
        if(!from || (strlen(from) == 0)) {
            __private::InvalidFieldValue<ToT>(to);
             return false;
        } 
        __private::fieldCast(from, to);
        return true;
   } catch (... ){
        __private::InvalidFieldValue<ToT>(to);
        return false;
   }    
}

inline bool fieldCast(  const char* from, char *to) { return false; }


template <typename KeyT, KeyT size>  struct Tags{
    Tags():m_tags(size){}
    const XMLSritgT& getTag(KeyT key) const {return m_tags.at(key); }
protected:     
    std::vector<XMLSritgT> m_tags;
};

inline TimeValueT ms2TimeValue( long ms ){
        if(ms == CONSTS::InvalidMSDuration) return CONSTS::InvalidTimeout;      
    return (TimeValueT(ms/1000, (ms%1000) * 1000));
}

inline long str2MSDuration( const char *time ){
        if(!time) return  CONSTS::InvalidMSDuration;
        int ms = CONSTS::InvalidMSDuration;
        try{
        ms = boost::lexical_cast<int>(time);
    }catch(boost::bad_lexical_cast ){
        ms = CONSTS::InvalidMSDuration;
    }
        return ms;
}

inline TimeValueT srt2TimeValue( const char *time ){
    if(!time) return  CONSTS::InvalidTimeout;
        return ms2TimeValue(str2MSDuration(time));      
}

inline std::string getLogNameHost(){ 
        const char *name = getenv("LOGNAME");
        if(!name) return CONSTS::InvalidLogNameHost;
        const  char *host = getenv("HOSTNAME");
        if(!host) return CONSTS::InvalidLogNameHost;
        std::string out(name);
        out +=  CONSTS::At;
        out += host;
        return out;
}

inline std::string getDalHost() { 
        const char *host = getenv("DAL_HOST");
        return (host) ? host : CONSTS::InvalidHost;
}
inline PortT getDalPort(){ //fieldCast
        const char *port = getenv("DAL_PORT");
        if(!port) return CONSTS::InvalidPort;
        try{
            return boost::lexical_cast<PortT>(port);
        }catch(boost::bad_lexical_cast& ){
            return CONSTS::InvalidPort;
        }
}

inline std::string getApplicationName() {
        const char *name = getenv("APPLICATION_NAME");
        return (name) ? name : CONSTS::EmptyStr;
}

inline std::string getUserId() {
        const char *id = getenv("USER_ID");
        return (id) ? id : CONSTS::EmptyStr;
}

inline int connectRetry()  {
        const char *retry = getenv("DAL_CONNECT_RETRY");
        if(!retry) return CONSTS::InvalidRetry;
        try{
            return boost::lexical_cast<int>(retry);
        }catch(boost::bad_lexical_cast& ){
            return CONSTS::InvalidRetry;
        }
}

inline std::string getExtractorLogFile() {
        const char *id = getenv("EXTRACTOR_LOG_FILE");
        std::string name = (id) ? id : CONSTS::EmptyStr;
        name += CONSTS::EXTRACTOR_LOG_SUFFIX;
        return name;
}

inline long connectTimeout(){
        return str2MSDuration(getenv("DAL_CONNECT_TIMEOUT"));            
}
        
inline long sendTimeout(){
        return str2MSDuration(getenv("DAL_SEND_TIMEOUT"));       
}

inline long receiveTimeout(){
        return str2MSDuration(getenv("DAL_RECEIVE_TIMEOUT"));    
}


/*
#ifdef BOOST_CAST_TOO_SLOW

template < typename ToT, typename FromT  > inline ToT fieldCast( FromT from){
        BOOST_STATIC_ASSERT(false );
}

template< > inline int fieldCast<int, char*>( char* from){ return atoi(from); }
template< > inline int fieldCast<int, const char*>( const char* from){ return atoi(from); }
template< > inline long fieldCast<long, char*>( char* from){ return atol(from); }
template< > inline long fieldCast<long, const char*>( const char*  from){ return atol(from); }

template< > inline int fieldCast<int, char*>( char* from){ return atoi(from); }
template< > inline int fieldCast<int, const char*>( const char* from){ return atoi(from); }
template< > inline long fieldCast<long, char*>( char* from){ return atol(from); }
template< > inline long fieldCast<long, const char*>( const char*  from){ return atol(from); }

template< > inline float fieldCast<float, char*>( char* from){ return atof(from); }
template< > inline float fieldCast<float, const char*>( const char* from){ return atof(from); }
template< > inline double fieldCast<double, char*>( char* from){ return atof(from); }
template< > inline double fieldCast<double, const char*>( const char*  from){ return atof(from); }
#else
*/



}//namespace DAL_IMPL{
#endif// _DALUTILS_H
