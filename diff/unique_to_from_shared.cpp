#include <type_traits>
#include <iostream>
#include <memory>
#include<string>
#include <tuple>
#include<typeinfo>

void print(std::shared_ptr<int> sp ){
	std::cout << *sp << std::endl;
}

void print(std::unique_ptr<int> up ){
	std::cout << *up << std::endl;
}

void unique_to_shared(std::unique_ptr<int> up){ // It is OK
	std::shared_ptr<int> sp(std::move(up));

	//unique_ptr is nullptr but nor deleted;
	bool isvalid(up);// false
	int* rp = up.get();// nullptr;
	// shared_ptr is valid
	int cnt = sp.use_count();
	print(sp);

}

template <typename ET> void shared_to_unique(std::shared_ptr<ET> sp){ // It is OK only if copy_constructible;
	using T = typename std::shared_ptr<ET>::element_type;
	static_assert( std::is_copy_constructible<T>::value, " must be copy_constructible ");
	std::unique_ptr<T> up = std::make_unique<T>(*sp);
	print(std::move(up));
	return;

}


int main(int argc, char** argv){
	std::unique_ptr<int> up = std::make_unique<int>(6);// It is OK
	unique_to_shared(std::move(up));

	std::shared_ptr<int> sp = std::make_shared<int>(int(11));
	shared_to_unique(sp);

 return 0;
}
