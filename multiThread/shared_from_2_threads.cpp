#include <thread>
#include <iostream>
#include <memory>



struct MyClass{
	void do_stuff(){

	}
};

void do_stuff_with(std::shared_ptr<MyClass> sp){

}
void thread_func(std::shared_ptr<MyClass> sp){
    sp->do_stuff();
    std::shared_ptr<MyClass> sp2=sp;
    do_stuff_with(sp2);
}
int main(){
    std::shared_ptr<MyClass> sp(new MyClass);
    std::thread thread1(thread_func,sp);
    std::thread thread2(thread_func,sp);
    thread1.join();
    thread2.join();
 
}

