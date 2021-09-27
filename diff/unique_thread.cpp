#include <thread>
#include <memory>


typedef std::string DT;
void f(std::unique_ptr<DT> ptr){
	int i = ptr->length();
	return;
}


int main()
{
std::unique_ptr<DT> ptr = std::make_unique<DT>("xxxxxxxxxxxx");
 std::thread th(f, std::move(ptr));
 th.join();

    return 0;
}
