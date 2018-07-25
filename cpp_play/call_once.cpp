#include <functional>
#include <iostream>
// call oonce per process ( not once per object)

struct CallWrapper {
	explicit CallWrapper(std::function<void(void)> f) {
		try {
			f();
		}
		catch (...) {

		}
	}

};


struct C {
	void stop_once() {
		static CallWrapper call([this] {stop(); });
	}
private:
	void stop() {
		std::cout << "stop()" << std::endl;
	}

};

int main()
{
	C c1;
	c1.stop_once();
	c1.stop_once();
	c1.stop_once();


}
