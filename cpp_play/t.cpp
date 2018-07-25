#include <iostream>
#include <string>
#include <memory>
#include <type_traits>
#include <boost/asio.hpp>

struct BinaryOstream{
	//BinaryOstream():
	//	m_buffer(std::make_shared<boost::asio::streambuf>() ),
	//	m_stream(m_buffer.get()){
	//}


	template<class T> typename std::enable_if<std::is_arithmetic<T>::value>::type  put(T data) {
	   m_stream.write(reinterpret_cast<const char*>(&data), sizeof(data));
	}
	void put(const char* data) {
		m_stream << data;
	}
	void put(const std::string& data) {
		m_stream << data;
	}
	bool finalize(){
		m_stream.flush();
		return !(m_stream.fail());
	}

	std::shared_ptr<boost::asio::streambuf> get_buffer(){
		return m_buffer;
	}
	size_t get_size() const{
		return m_buffer->size();
	}
private:
	std::shared_ptr<boost::asio::streambuf> 	m_buffer{std::make_shared<boost::asio::streambuf>() };
	std::ostream 		 						m_stream{m_buffer.get()};
};

struct BinaryIstream{

	BinaryIstream():
		m_buffer(std::make_shared<boost::asio::streambuf>() ),
		m_stream(m_buffer.get()){
	}

	BinaryIstream(std::shared_ptr<boost::asio::streambuf> buffer):
			m_buffer(buffer ),
			m_stream(m_buffer.get()){
		}


	template<class T> typename std::enable_if<std::is_arithmetic<T>::value>::type  get(T &data) {
	   m_stream.read(reinterpret_cast<char*>(&data), sizeof(data));
	}
	void get(char* data) {
		m_stream >> data;
	}
	void get(std::string& data) {
		m_stream >> data;
	}
	bool finalize(){
		return m_stream.good();
	}
	std::shared_ptr<boost::asio::streambuf> get_buffer(){
			return m_buffer;
		}
	size_t get_size() const{
		return m_buffer->size();
	}
private:
	std::shared_ptr<boost::asio::streambuf> 		m_buffer;
	std::istream 		 							m_stream;
};



int main(int, char*[])
{
	BinaryOstream os;
	int i1 = 1;
	os.put(i1);
	float f1 = 2.4;
	os.put(f1);
	os.put("123456");


	bool status = os.finalize();

	BinaryIstream is(os.get_buffer());
	int i2 =0;
	is.get(i2);
	float f2 = 0.0;
	is.get(f2);
	std::string s;
	is.get(s);
	status = is.finalize();

	std::cout <<i2 <<":"<< f2 <<":"<<s<<std::endl;


    return 0;
}
