

#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <memory>
#include <mutex>

struct X {
	static X* instance(){
		std::call_once(m__init_flag,init);
		return m_instance;
	}
private:
  static void init(){
    m_instance = new X;
  }
	X(){ std::cout <<"X()"<< std::endl; }
	X(const X&){}

	static std::once_flag   m__init_flag;
  static X*               m_instance;
};
std::once_flag X::m__init_flag;
X* X::m_instance(NULL);


int main()
{
	X *x1 = X::instance();
	X *x2 = X::instance();
  return 0;
}

