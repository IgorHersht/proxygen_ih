#include <atomic>
#include <string>
#include <memory>



int main()
{
	std::string ss("xxxxxx");
	std::shared_ptr<std::string> s1(new std::string("xxxxx"));
	std::shared_ptr<std::string> s2(new std::string("yyyy"));
	std::atomic_store(&s1, s2);


	return 0;
}

