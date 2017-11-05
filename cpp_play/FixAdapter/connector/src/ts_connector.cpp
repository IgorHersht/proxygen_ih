#include <boost/asio/error.hpp>
#include <boost/asio/socket_base.hpp>
#include <boost/bind.hpp>
#include "Const.h"
#include "ts_connector.h"


namespace other_fix {

void TsConnector::connect_not_locked()
{
    if( connected() ) return;
    std::string endPoint = m_ipAdress; endPoint += ":"; endPoint += m_port;
    try{
        tcp::resolver resolver(m_io_service);
        tcp::resolver::query query(m_ipAdress, m_port);
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
   
        logxxx(std::string("Trying to connect to: ") + endPoint);

        boost::system::error_code error = boost::asio::error::host_not_found;
        connect_not_locked(endpoint_iterator, error );
        if(error) {
             const int reconnectIn = m_settings.getConnectionIntervaMillis();
             const std::string reconnectInStr = boost::lexical_cast<std::string>(reconnectIn);
             std::string msg = "Cannot connect to the trading system ";
             msg += endPoint; msg += ". "; msg += error.message(); 
             msg += ". Reconnect in  "; msg += reconnectInStr;  msg += " milliseconds.";
             logxxx(msg);
        }

        while(error){
             const int reconnectIn = m_settings.getConnectionIntervaMillis();
             boost::this_thread::sleep(boost::posix_time::milliseconds(reconnectIn));
             connect_not_locked(endpoint_iterator, error );
         }

         setOptions();
         logxxx(std::string("Connected to: ") + endPoint);
    }catch(std::exception &er){
        std::string msg = "Cannot connect to the trading system ";
        msg += endPoint;  msg += ". ";
        msg += er.what();  msg += ". ";
        disconnect_not_locked();
        throw std::runtime_error(msg);
    }catch (...){
       std::string msg = "Cannot connect to the trading system ";
       msg += endPoint;  msg += ". ";
       disconnect_not_locked();
       throw std::runtime_error(msg);
    }
}

void TsConnector::connect_not_locked(tcp::resolver::iterator endpoint_iterator, boost::system::error_code &error){
     
     tcp::resolver::iterator end;
     while (error && endpoint_iterator != end){
            disconnect_not_locked();
            m_socket.connect(*endpoint_iterator++, error);
      }

}
void  TsConnector::disconnect_not_locked(){
    if(!connected()) {
        return;
    }
   boost::system::error_code  ec  = boost::asio::error::bad_descriptor;
    try{
        m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
        if(ec && (ec != boost::asio::error::not_connected) && (ec != boost::asio::error::bad_descriptor)  ) {
            std::string msg = "An error on socket::shutdown_send. "; msg += ec.message();
            warningxxx(msg);
        }
        m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_receive, ec);
        if(ec && (ec !=boost::asio::error::not_connected) && (ec != boost::asio::error::bad_descriptor)) {
            std::string msg = "An error on socket::shutdown_receive. "; msg += ec.message();
           warningxxx(msg);
        }
         m_socket.close(ec);
         if(ec && (ec !=boost::asio::error::not_connected) && (ec != boost::asio::error::bad_descriptor) ) {
             std::string msg = "An error on socket::close. "; msg += ec.message();
             warningxxx(msg);
         }

    }catch (...){
        warningxxx("An error on socket disconnect" );
    }   
}

bool  TsConnector::connected() const {
    return m_socket.is_open();
}

void TsConnector::setOptions() {
  using boost::asio::socket_base;
  set_socket_option(tcp::no_delay(m_settings.getNoDelay()),  "no_delay");
  set_socket_option(socket_base::send_buffer_size(m_settings.getSendBufferSize()), "send_buffer_size");
  set_socket_option(socket_base::receive_buffer_size(m_settings.getRcvedBufferSize()), "rcvd_buffer_size");
  set_socket_option(socket_base::keep_alive(m_settings.getKeepAlive()),"keep_alive");
  set_socket_option(socket_base::linger(m_settings.getSoLinger().first, m_settings.getSoLinger().second),"linger");
}

std::string TsConnector::to_string() const {
  std::ostringstream stream;

  stream << (connected() ? "Connected to " : "Not connected to ")
      << m_settings.getAddress() << ":" << m_settings.getPort();
  if (!connected()) return stream.str();

  using boost::asio::socket_base;
  {
    tcp::no_delay o;
    record_current_socket_option(o,  "no_delay", PREPEND, stream);
  }
  {
    socket_base::send_buffer_size o;
    record_current_socket_option(o, "send_buffer_size", PREPEND, stream);
  }
  {
    socket_base::receive_buffer_size o;
    record_current_socket_option(o,  "rcvd_buffer_size", PREPEND, stream);
  }
  {
    socket_base::keep_alive o;
    record_current_socket_option(o, "keep_alive", PREPEND, stream);
  }
  {
    socket_base::linger o;
    boost::asio::socket_base::linger option;
    m_socket.get_option(option);
    stream << ";isLinger = " << option.enabled() << "; lingerTime = " << option.timeout();
  }
  return stream.str();
}

}  //namespace other_fix
