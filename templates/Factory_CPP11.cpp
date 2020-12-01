

#include <iostream>
#include <type_traits>
#include<memory>
#include<string>
#include<vector>



struct  Base {
	virtual ~Base() {};
	virtual void run() = 0;
	virtual void close() = 0;
};

struct Impl1 : public Base {
	explicit Impl1(int data) :Base(), m_data(data) { std::cout << " Impl1()" << std::endl; }
	virtual ~Impl1() { std::cout << " ~Impl1()" << std::endl; }
	virtual void run() override { std::cout << " Impl1::run() with " << m_data << std::endl; }
	virtual void close() override { std::cout << " Impl1::close()" << std::endl; }
private:
	int m_data;
};

struct Impl2 : public Base {
	explicit Impl2(std::string data) :Base(), m_data(data) { std::cout << " Impl2()" << std::endl; }
	virtual ~Impl2() { std::cout << " ~Impl2()" << std::endl; }
	virtual void run() override { std::cout << " Impl2::run()with " << m_data << std::endl; }
	virtual void close() override { std::cout << " Impl2::close()" << std::endl; }
private:
	std::string m_data;
};



template <typename BaseT> class Factory {
	static_assert(std::has_virtual_destructor<BaseT>::value, "base should have virtual_destructor ");
	struct deleter {
		void operator ()(BaseT * base) {
			if (base) {
				base->close();
				delete base;
			}
		}
	};
	/*  need C++14
	static auto deleter;
	};
	*/
public:
	using ReturnT = std::unique_ptr<BaseT, deleter >;
	template <typename ImplT, typename ... ArgsT>
	static ReturnT make(ArgsT&& ... args) {
		static_assert(std::is_base_of<BaseT, ImplT>::value, "wrong impl type");
		static_assert(std::is_constructible<ImplT, ArgsT...>::value, "wrong impl constructor");
		return ReturnT(new ImplT(std::forward<ArgsT>(args)...));
	}


};

int main(int argc, char* argv[]) {
	{
		auto  impl1(Factory<Base>::make<Impl1>(5));
		impl1->run();
		auto  impl2(Factory<Base>::make<Impl2>(std::string("xxxx")));
		impl2->run();
		std::vector<decltype(impl1)> v;
		v.push_back(std::move(impl1));
		v.push_back(std::move(impl2));
	}
	int i = 1;

	return 0;
}
/*
  Impl1()
 Impl1::run() with 5
 Impl2()
 Impl2::run()with xxxx
 Impl2::close()
 ~Impl2()
 Impl1::close()
 ~Impl1()

 
 
 */
 
