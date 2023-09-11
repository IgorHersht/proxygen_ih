
#include <iostream>
#include <queue>
#include <thread>
#include <stop_token>
#include <mutex>
#include <condition_variable>
using namespace std::literals;  // for duration literals

int main()
{
  std::queue<std::string> messages;
  std::mutex messagesMx;
  std::condition_variable_any messagesCV;  

  // start thread that prints messages that occur in the queue:
  std::jthread t1{[&] (std::stop_token st) {
                    while (!st.stop_requested()) {
                      std::string msg;
                      {
                        // wait for the next message:
                        std::unique_lock lock(messagesMx);
                        if (!messagesCV.wait(lock, st,
                                             [&] { 
                                               return !messages.empty();
                                             })) {
                          return;  // stop requested
                        }
                        // retrieve the next message out of the queue:
                        msg = messages.front();
                        messages.pop();
                      }

                      // print the next message:
                      std::cout << "msg: " << msg << std::endl;
                    }
                  }};

  // store 3 messages and notify one waiting thread each time:
  for (std::string s : {"Tic", "Tac", "Toe"}) {
    std::scoped_lock lg{messagesMx};
    messages.push(s);
    messagesCV.notify_one();
  }

  // after some time
  // - store 1 message and notify all waiting threads:
  std::this_thread::sleep_for(1s);
  {
    std::scoped_lock lg{messagesMx};
    messages.push("done");
    messagesCV.notify_all();
  }

  // after some time
  // - end program (requests stop, which interrupts wait())
  std::this_thread::sleep_for(1s);
}


