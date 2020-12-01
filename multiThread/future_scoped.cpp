#include <future>
#include <functional>
#include <chrono>
#include <optional>

struct ScopedFuture{
  ScopedFuture() = default;
  explicit ScopedFuture( std::function<void()> f):
    m_future(std::async(std::launch::async, f) ){
  }
  ScopedFuture( std::function<void()> f, unsigned long timeout):
    m_future(std::async(std::launch::async, f) ),
    m_timeout(timeout){
  }

  ScopedFuture( ScopedFuture&& sf):
    m_future(std::move(sf.m_future)),
    m_timeout(std::move(sf.m_timeout)){

    }
  ScopedFuture& operator =( ScopedFuture&& sf){
    if(&sf == this){
      return *this;
    }
    m_future = std::move(sf.m_future);
    m_timeout = std::move(sf.m_timeout);
    return *this;
  }

// to catch exeptions and in the working threads and react on timeout
  ~ScopedFuture(){
    destruct();
  }
private:
  void destruct(){
    try{
        if(!m_future.valid()){
          return;
        }

        if(!m_timeout){
          m_future.get();
          return;
        }

        auto status = m_future.wait_for(std::chrono::milliseconds(*m_timeout));
        if(status == std::future_status::timeout){
          // what to do on timeout
          return;
        }
        m_future.get();

      }catch(...)
      {
        // what to do on exception
      }
  }

private:
  std::future<void>               m_future;
  std::optional<unsigned long>    m_timeout;
};

//test
#include <iostream>


void foo()
{
  std::cout << "start foo" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(5));
  std::cout << "end foo" << std::endl;
}

int main()
{
  {
    ScopedFuture m_sf;
    {
      ScopedFuture sf(foo);
      m_sf = std::move(sf);
    }
  }

  {
    ScopedFuture m_sf;
    {
      ScopedFuture sf(foo, 2000 );
      m_sf = std::move(sf);
    }
  }



  return 0;
}
/*
start foo
end foo
 */

