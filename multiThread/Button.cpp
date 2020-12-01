#include <thread>
#include <mutex>
#include <condition_variable>

struct Button{
  void wait(){
    std::unique_lock<std::mutex> lc(m_m);
    while(!m_flag){
      m_set.wait(lc);
    }
  }
  void set(){
    std::unique_lock<std::mutex> lc(m_m);
    m_flag = true;
    m_set.notify_all();
  }
private:
  bool m_flag{ false };
  mutable std::mutex m_m;
  std::condition_variable m_set;
};
//// test
Button button;
void set() {
  std::this_thread::sleep_for(std::chrono::seconds(10));
  button.set();
  std::this_thread::sleep_for(std::chrono::seconds(2));
}

void wait() {
  int i = 0;
  button.wait();
  i = 1;
}
int main(int, char*[])
{
  std::thread tw1(wait); std::thread tw2(wait);
  std::thread ts(set);


  tw1.join(); tw2.join(); ts.join();

  return 0;
}


