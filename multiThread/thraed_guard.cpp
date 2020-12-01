#include <thread>
#include <stack>
#include <mutex>
#include <memory>
#include <atomic>
#include <iostream>

 struct ThreadGuard{
    ThreadGuard(std::thread& th):m_th(th){}
   ~ThreadGuard(){
     if(m_th.joinable()) {
       m_th.join();
     }
   }
   ThreadGuard(const ThreadGuard&) = delete;
   ThreadGuard& operator = (const ThreadGuard&) = delete;
 private:
     std::thread& m_th;
 };


 struct func {

    void operator()()
    {
        for(unsigned j=0;j<1000000;++j)
        {
            std::cout  <<j<<std::endl;
        }
    }
};



int main(int argc, char* argv[]){
  func f;
  {
      std::thread t(f);
      ThreadGuard guard(t);
  }
  return 0;
}

