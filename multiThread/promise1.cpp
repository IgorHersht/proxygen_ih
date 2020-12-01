#include <thread>
#include <future>
#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>
#include <functional>
#include <utility>

void doSomething (std::promise<std::string>& p)
{
    try {
        // read character and throw exceptiopn if 'x'
        std::cout << "read char ('x' for exception): ";
        char c = 'x';
        if (c == 'x') {
            throw std::runtime_error(std::string("char ")+c+" read");
        }
        //...
        std::string s = std::string("char ") + c + " processed";
        p.set_value(std::move(s));    // store result
    }
    catch (...) {
        p.set_exception(std::current_exception());  // store exception
    }
}

int main()
{
    try {
        // start thread using a promise to store the outcome
        std::promise<std::string> p;
        std::thread t(doSomething,std::ref(p));
        t.detach();
        //...

        // create a future to process the outcome
        std::future<std::string> f(p.get_future());

        // process the outcome
        std::cout << "result: " << f.get() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "EXCEPTION " << std::endl;
    }
}
