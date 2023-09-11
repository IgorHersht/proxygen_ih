#include <iostream>
#include <stop_token>
#include <future>     // for std::async()
#include <thread>     // for sleep_for()
#include <syncstream> // for std::osyncstream
#include <chrono>
using namespace std::literals;  // for duration literals

auto syncOut(std::ostream& strm = std::cout) {
    return std::osyncstream{strm};
}

void task(std::stop_token st, int num)
{
    auto id = std::this_thread::get_id();
    syncOut() << "call task(" << num << ")\n";

    // register a first callback:
    std::stop_callback cb1{st, [num, id]{
        syncOut() << "- STOP1 requested in task(" << num
                  << (id == std::this_thread::get_id() ?  ")\n"
                                                       :  ") in main thread\n");
    }};
    std::this_thread::sleep_for(9ms);

    // register a second callback:
    std::stop_callback cb2{st, [num, id]{
        syncOut() << "- STOP2 requested in task(" << num
                  << (id == std::this_thread::get_id() ?  ")\n"
                                                       :  ") in main thread\n");
    }};
    std::this_thread::sleep_for(2ms);
}

int main()
{
    // create stop_source and stop_token:
    std::stop_source ssrc;
    std::stop_token stok{ssrc.get_token()};

    // register callback:
    std::stop_callback cb{stok, []{
        syncOut() << "- STOP requested in main()\n" << std::flush;
    }};

    // in the background call task() a bunch of times:
    auto fut = std::async(std::launch::async, [stok] {
        for (int num = 1; num < 10; ++num) {
            task(stok, num);
        }
    });

    // after a while, request stop:
    std::this_thread::sleep_for(1ms);
    ssrc.request_stop();
}


