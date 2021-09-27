#include <string>
#include <map>
#include <boost/lexical_cast.hpp>
#include <iostream>


int main() {
	bool v1 =  boost::lexical_cast<bool>(std::string("1"));
	bool v2 =  boost::lexical_cast<bool>(std::string("0"));
	//bool v2 =  boost::lexical_cast<bool>(std::string("10")); only "0" or "1"
	std::string s1 = boost::lexical_cast<std::string >(true);
	std::string s2 = boost::lexical_cast<std::string >(false);
	/////
	v1 =  boost::lexical_cast<bool>(s2);
	v2 =  boost::lexical_cast<bool>(s1);
	 s1 = boost::lexical_cast<std::string >(v1);
	 s2 = boost::lexical_cast<std::string >(v2);





return 0;
}

