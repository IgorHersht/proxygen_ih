#include <thread>
#include <memory>

typedef std::shared_ptr<std::thread> ThreadPtr;

template< typename Function, typename ... Args> ThreadPtr makeThreadPtr(Function&& f, Args&& ... args){

  auto deleter = [](std::thread* th){
    if(!th){
      return;
    }
    try{
      th->join();
    }catch(...){}
    delete th;
  };

return ThreadPtr( new std::thread(f, std::forward<Args>(args) ...), deleter );

}

//test

void foo(int i){
  int j =i;
}


int main() {
  {
    ThreadPtr ptr = makeThreadPtr(foo,5);
    int i =1;
  }
int i =1;


}

