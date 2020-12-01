/*
 * ClientOptions.h
 *
 *  Created on: Sep 10, 2015
 *      Author: ihersht
 */
#pragma once


#include <boost/shared_ptr.hpp>
#include <cpp_tools/Options.h>


namespace rf{
namespace sh{

enum OptionKey{
	////Zookeeper,
	ZOOKEEPER_SERVER,
	ZOOKEEPER_PATH,
	ZOOKEEPER_LOG_FILE,
	ZOOKEEPER_RELOAD_DELAY,
	ZOOKEEPER_RELOAD_RETRY,
	ZOOKEEPER_RECV_TIMEOUT,
	ZOOKEEPER_LOG_LEVEL,
	//// thrift common
	THRIFT_SOCKET_TYPE, //Not visible to user, because the only one implemented now .
	THRIFT_TRANSPORT_TYPE, //Not visible to user, because the only one implemented now .
	THRIFT_PROTOCOL_TYPE, //Not visible to user, because the only one implemented now .
	THRIFT_CLIENTS_NUMBER,
	// thrift socket options
	THRIFT_RECONNECT_RETRY,
	THRIFT_RECONNECT_DELAY,
	THRIFT_USE_ZOO, //Not visible to user, because because used only for internal testing .
	THRIFT_HOST, //Not visible to user, because because used only for internal testing .
	THRIFT_PORT, //Not visible to user, because because used only for internal testing .
	THRIFT_CONNECT_TIMEOUT,
	THRIFT_SEND_TIMEOUT,
	THRIFT_RECV_TIMEOUT,
	THRIFT_KEEP_ALIVE,
	THRIFT_LINGER_ON,
	THRIFT_LINGER_VAL,
	THRIFT_NO_DELAY,
	THRIFT_MAX_RECV_RETRIES,
	// thrift recovery
	THRIFT_RETRY_ON_TIMEOUT,
	THRIFT_MAX_RECOVER_RETRIES,
	THRIFT_RESEND_ON_REREGISTER,
	THRIFT_RESEND_ON_RECOVERABLE_EXCEPTION,
	THRIFT_RESEND_ON_RECONNECT,
	THRIFT_REQUEST_FAIL_FAST,
	THRIFT_REQUEST_SKIP_BLOCKING_ON_REMOTES,
	THRIFT_USE_LOAD_BALANCER,
	THRIFT_LOAD_BALANCER_DELAY,
	_OPTION_KEY_NUM_ //Not visible to user

};

template<  int key> struct TraitDefault{
	enum {m_key = key};
	enum LoadPolicy{CAN_LOAD, MUST_LOAD, MUST_NOT_LOAD};
	static LoadPolicy getLoadPolicy(){return CAN_LOAD ;}
};
template<  int key> struct ClientOptionTrait;


template <> struct  ClientOptionTrait< ZOOKEEPER_SERVER> : public TraitDefault<ZOOKEEPER_SERVER> {
	static std::string string_key(){return "ZOOKEEPER_SERVER";}
	typedef std::string value_type;
	static value_type default_value(){ return "" ;}
	static LoadPolicy getLoadPolicy(){return MUST_LOAD ;}
};

template <> struct  ClientOptionTrait< ZOOKEEPER_PATH> : public TraitDefault<ZOOKEEPER_PATH> {
	static std::string string_key(){return "ZOOKEEPER_PATH";}
	typedef std::string value_type;
	static value_type default_value(){ return "" ;}
	static LoadPolicy getLoadPolicy(){return MUST_LOAD ;}
};

template <> struct  ClientOptionTrait< ZOOKEEPER_LOG_FILE> : public TraitDefault<ZOOKEEPER_LOG_FILE> {
	static std::string string_key(){return "ZOOKEEPER_LOG_FILE";}
	typedef std::string value_type;
	static value_type default_value(){ return "" ;}
	static LoadPolicy getLoadPolicy(){return MUST_LOAD ;}
};

template <> struct  ClientOptionTrait< ZOOKEEPER_RELOAD_DELAY> : public TraitDefault<ZOOKEEPER_RELOAD_DELAY> {
	static std::string string_key(){return "ZOOKEEPER_RELOAD_DELAY";}
	typedef int value_type;
	static value_type default_value(){ return 1000 ;}
};
template <> struct  ClientOptionTrait< ZOOKEEPER_RELOAD_RETRY> : public TraitDefault<ZOOKEEPER_RELOAD_RETRY> {
	static std::string string_key(){return "ZOOKEEPER_RELOAD_RETRY";}
	typedef long value_type;
	static value_type default_value(){ return 86400 ;}
};
template <> struct  ClientOptionTrait< ZOOKEEPER_RECV_TIMEOUT > : public TraitDefault<ZOOKEEPER_RECV_TIMEOUT> {
	static std::string string_key(){return "ZOOKEEPER_RECV_TIMEOUT";}
	typedef int value_type;
	static value_type default_value(){ return 1000 ;}
};
template <> struct  ClientOptionTrait< ZOOKEEPER_LOG_LEVEL> : public TraitDefault<ZOOKEEPER_LOG_LEVEL> {
	static std::string string_key(){return "ZOOKEEPER_LOG_LEVEL";}
	typedef std::string value_type;
	static value_type default_value(){ return "ERR" ;}
};
template <> struct  ClientOptionTrait< THRIFT_SOCKET_TYPE> : public TraitDefault<THRIFT_SOCKET_TYPE> {
	static std::string string_key(){return "THRIFT_SOCKET_TYPE";}
	typedef std::string value_type;
	static value_type default_value(){ return "TSocket" ;}
	static LoadPolicy getLoadPolicy(){return MUST_NOT_LOAD ;}
};
template <> struct  ClientOptionTrait< THRIFT_TRANSPORT_TYPE> : public TraitDefault<THRIFT_TRANSPORT_TYPE> {
	static std::string string_key(){return "THRIFT_TRANSPORT_TYPE";}
	typedef std::string value_type;
	static value_type default_value(){ return "TFramedTransport" ;}
	static LoadPolicy getLoadPolicy(){return MUST_NOT_LOAD ;}
};
template <> struct  ClientOptionTrait< THRIFT_PROTOCOL_TYPE> : public TraitDefault<THRIFT_PROTOCOL_TYPE> {
	static std::string string_key(){return "THRIFT_PROTOCOL_TYPE";}
	typedef std::string value_type;
	static value_type default_value(){ return "TBinaryProtocol" ;}
	static LoadPolicy getLoadPolicy(){return MUST_NOT_LOAD ;}
};
template <> struct  ClientOptionTrait< THRIFT_CLIENTS_NUMBER> : public TraitDefault<THRIFT_CLIENTS_NUMBER> {
	static std::string string_key(){return "THRIFT_CLIENTS_NUMBER";}
	typedef std::size_t value_type;
	static value_type default_value(){ return 100 ;}
};
template <> struct  ClientOptionTrait< THRIFT_RECONNECT_RETRY> : public TraitDefault<THRIFT_RECONNECT_RETRY> {
	static std::string string_key(){return "THRIFT_RECONNECT_RETRY";}
	typedef long value_type;
	static value_type default_value(){ return ClientOptionTrait< THRIFT_CLIENTS_NUMBER>::default_value();}
};
template <> struct  ClientOptionTrait< THRIFT_RECONNECT_DELAY> : public TraitDefault<THRIFT_RECONNECT_DELAY> {
	static std::string string_key(){return "THRIFT_RECONNECT_DELAY";}
	typedef long value_type;
	static value_type default_value(){ return 1000 ;}
};
template <> struct  ClientOptionTrait< THRIFT_USE_ZOO> : public TraitDefault<THRIFT_USE_ZOO> {
	static std::string string_key(){return "THRIFT_USE_ZOO";}
	typedef bool value_type;
	static value_type default_value(){ return true ;}
};
template <> struct  ClientOptionTrait< THRIFT_HOST> : public TraitDefault<THRIFT_HOST> {
	static std::string string_key(){return "THRIFT_HOST";}
	typedef std::string value_type;
	static value_type default_value(){ return "localhost" ;}
};
template <> struct  ClientOptionTrait< THRIFT_PORT> : public TraitDefault<THRIFT_PORT> {
	static std::string string_key(){return "THRIFT_PORT";}
	typedef int value_type;
	static value_type default_value(){ return 2181 ;}
};
template <> struct  ClientOptionTrait< THRIFT_CONNECT_TIMEOUT> : public TraitDefault<THRIFT_CONNECT_TIMEOUT> {
	static std::string string_key(){return "THRIFT_CONNECT_TIMEOUT";}
	typedef int value_type;
	static value_type default_value(){ return 10000 ;}
};
template <> struct  ClientOptionTrait< THRIFT_SEND_TIMEOUT> : public TraitDefault<THRIFT_SEND_TIMEOUT> {
	static std::string string_key(){return "THRIFT_SEND_TIMEOUT";}
	typedef int value_type;
	static value_type default_value(){ return 1000 ;}
};
template <> struct  ClientOptionTrait< THRIFT_RECV_TIMEOUT> : public TraitDefault<THRIFT_RECV_TIMEOUT> {
	static std::string string_key(){return "THRIFT_RECV_TIMEOUT";}
	typedef int value_type;
	static value_type default_value(){ return 1000 ;}
};
template <> struct  ClientOptionTrait< THRIFT_KEEP_ALIVE> : public TraitDefault<THRIFT_KEEP_ALIVE> {
	static std::string string_key(){return "THRIFT_KEEP_ALIVE";}
	typedef bool value_type;
	static value_type default_value(){ return true;}
};
template <> struct  ClientOptionTrait< THRIFT_LINGER_ON> : public TraitDefault<THRIFT_LINGER_ON> {
	static std::string string_key(){return "THRIFT_LINGER_ON";}
	typedef bool value_type;
	static value_type default_value(){ return false ;}
};
template <> struct  ClientOptionTrait< THRIFT_LINGER_VAL> : public TraitDefault<THRIFT_LINGER_VAL> {
	static std::string string_key(){return "THRIFT_LINGER_VAL";}
	typedef int value_type;
	static value_type default_value(){ return 1 ;}
};
template <> struct  ClientOptionTrait< THRIFT_NO_DELAY> : public TraitDefault<THRIFT_NO_DELAY> {
	static std::string string_key(){return "THRIFT_NO_DELAY";}
	typedef bool value_type;
	static value_type default_value(){ return true;}
};
template <> struct  ClientOptionTrait< THRIFT_MAX_RECV_RETRIES> : public TraitDefault<THRIFT_MAX_RECV_RETRIES> {
	static std::string string_key(){return "THRIFT_MAX_RECV_RETRIES";}
	typedef int value_type;
	static value_type default_value(){ return 5 ;}
};
/////////////////
template <> struct  ClientOptionTrait< THRIFT_RETRY_ON_TIMEOUT> : public TraitDefault<THRIFT_RETRY_ON_TIMEOUT> {
	static std::string string_key(){return "THRIFT_RETRY_ON_TIMEOUT";}
	typedef long value_type;
	static value_type default_value(){ return 20 ;}
};
template <> struct  ClientOptionTrait< THRIFT_MAX_RECOVER_RETRIES> : public TraitDefault<THRIFT_MAX_RECOVER_RETRIES> {
	static std::string string_key(){return "THRIFT_MAX_RECOVER_RETRIES";}
	typedef int value_type;
	static value_type default_value(){ return 10 ;}
};
template <> struct  ClientOptionTrait< THRIFT_RESEND_ON_REREGISTER> : public TraitDefault<THRIFT_RESEND_ON_REREGISTER> {
	static std::string string_key(){return "THRIFT_RESEND_ON_REREGISTER";}
	typedef bool value_type;
	static value_type default_value(){ return false ;}
};
template <> struct  ClientOptionTrait< THRIFT_RESEND_ON_RECOVERABLE_EXCEPTION> : public TraitDefault<THRIFT_RESEND_ON_RECOVERABLE_EXCEPTION> {
	static std::string string_key(){return "THRIFT_RESEND_ON_RECOVERABLE_EXCEPTION";}
	typedef bool value_type;
	static value_type default_value(){ return true ;}
};
template <> struct  ClientOptionTrait< THRIFT_RESEND_ON_RECONNECT> : public TraitDefault<THRIFT_RESEND_ON_RECONNECT> {
	static std::string string_key(){return "THRIFT_RESEND_ON_RECONNECT";}
	typedef bool value_type;
	static value_type default_value(){ return false ;}
};
template <> struct  ClientOptionTrait< THRIFT_REQUEST_FAIL_FAST> : public TraitDefault<THRIFT_REQUEST_FAIL_FAST> {
	static std::string string_key(){return "THRIFT_REQUEST_FAIL_FAST";}
	typedef bool value_type;
	static value_type default_value(){ return true ;}
};
template <> struct  ClientOptionTrait< THRIFT_REQUEST_SKIP_BLOCKING_ON_REMOTES> : public TraitDefault<THRIFT_REQUEST_SKIP_BLOCKING_ON_REMOTES> {
	static std::string string_key(){return "THRIFT_REQUEST_SKIP_BLOCKING_ON_REMOTES";}
	typedef bool value_type;
	static value_type default_value(){ return true ;}
};
template <> struct  ClientOptionTrait< THRIFT_USE_LOAD_BALANCER> : public TraitDefault<THRIFT_USE_LOAD_BALANCER> {
	static std::string string_key(){return "THRIFT_USE_LOAD_BALANCER";}
	typedef bool value_type;
	static value_type default_value(){ return false ;}
};
template <> struct  ClientOptionTrait< THRIFT_LOAD_BALANCER_DELAY> : public TraitDefault<THRIFT_LOAD_BALANCER_DELAY> {
	static std::string string_key(){return "THRIFT_LOAD_BALANCER_DELAY";}
	typedef int value_type;
	static value_type default_value(){ return 60000 ;}
};


struct ClientOptions;
typedef boost::shared_ptr<ClientOptions>	ClientOptionPtr;

class  ClientOptions{
	typedef cpp_tools::Options<_OPTION_KEY_NUM_, ClientOptionTrait > OptionsImpl;
	typedef boost::shared_ptr<OptionsImpl>	OptionsImplPtr;
public:
	static ClientOptionPtr Instance( cpp_tools::OptionLoader loader){

		OptionsImplPtr impl(boost::make_shared<OptionsImpl>(loader ));
		if(!impl){
			std::string msg = SRC_LOCATION + "null OptionsImplPtr." ;
			cpp_tools::log(msg ,cpp_tools::ERR);
			return ClientOptionPtr();
		}

		ClientOptionPtr clientOptions (new ClientOptions(impl ));
		if(!clientOptions){
			std::string msg = SRC_LOCATION + "null ClientOptionPtr " ;
			cpp_tools::log(msg ,cpp_tools::ERR);
			return ClientOptionPtr();
		 }
		bool status = clientOptions->init();
		if(!status){
			std::string msg = SRC_LOCATION + "Cannot init  ClientOptions. " ;
			cpp_tools::log(msg ,cpp_tools::ERR);
			clientOptions.reset();

		}
		return clientOptions;

	}
	template <int pos>  typename ClientOptionTrait<pos>::value_type getOption() const{
		return m_impl->getOption<pos>();
	}
private:
	explicit ClientOptions(OptionsImplPtr impl):m_impl(impl){

	}

	bool init(){
		if(!m_impl){
			return false;
		}
		try{
			if(!m_impl->init()){
				return false;
			}
			if(!m_impl->loadValues()){
				return false;
			}
			//m_impl->dump();
			return true;
		}catch(...){
			std::string msg = SRC_LOCATION + "unknown exception.";
			cpp_tools::log(msg ,cpp_tools::ERR);
			return false;
		}
	}
private:
	OptionsImplPtr 			m_impl;

};


}}
