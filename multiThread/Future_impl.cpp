#include <functional>
#include <exception>
#include <thread>
#include <variant>
#include <mutex>
#include <memory>
#include <future>
#include <condition_variable>
#include <boost/thread/sync_queue.hpp>

struct ThreadGuard{
    ThreadGuard(std::thread& th):_th(th){}
    ~ThreadGuard(){
        if(_th.joinable()){
            try{ _th.join();}
            catch(...){}
        }
    }
    ThreadGuard(const ThreadGuard& th) = delete;
    ThreadGuard& operator =(const ThreadGuard& th) = delete;
private:
    std::thread& _th;
};
struct Dummy{};
template<typename T> struct VoidTrait{ typedef T type; };
template<> struct VoidTrait<void>{ typedef Dummy type; };

template <typename T> class Future {
    using value_type = std::variant<typename VoidTrait<T>::type , std::exception_ptr>;
    template <typename F, typename ... Args> friend auto async(F&& f, Args&& ... args  );
public:
    decltype(auto) get()
    {
        std::unique_lock lc(_m);
        while(!_isSet){ _conSet.wait(lc); }
        if constexpr (!std::is_void_v<T>) {
            if(std::holds_alternative<T>(_value)){
                return std::get<T>(_value);
            }else {
                std::exception_ptr eptr = std::get<std::exception_ptr>(_value);
                if(eptr){
                    std::rethrow_exception(eptr);
                }
            }
        } else{
            std::exception_ptr eptr = std::get<std::exception_ptr>(_value);
            if(eptr){
                std::rethrow_exception(eptr);
            }
            return;
        }

    }

    template <typename F> Future(F&& f )
    {
        std::thread th([this, f = std::forward<F>(f) ]{ callWrapper(f) ;}) ;
        _th = std::move(th);
    }

private:
    template <typename F> void callWrapper(F&& f){
        std::unique_lock lc(_m);
        setValue(std::forward<F>(f));
        _isSet = true;
        _conSet.notify_all();
    }

    template <typename F> void setValue (F&& f)
    {
        std::exception_ptr eptr;
        _value = eptr;
        try{
            if constexpr (std::is_void_v<T>) {
                f();
            }else{
                _value = f();
            }
        }catch(...){
            _value =std::current_exception();
        }
    }
private:
    std::thread                 _th;
    ThreadGuard                 _threadGuard{_th} ;
    mutable std::mutex          _m;
    std::condition_variable     _conSet;
    bool                      _isSet{false};
    value_type                 _value;
};

template <typename F, typename ... Args> auto Async(F&& f, Args&& ... args  ){
    auto l = [f = std::forward<F>(f),... args = std::forward<Args>(args)](){// c+=20 only?
        return f(args ...);
    };
    return Future<std::invoke_result_t<decltype(l)> >(l);
}

//////////////////// test

void foo0(int i){
    int j  =i;
}
void foo01(){
    throw std::string ("void f01()");
}

int foo1() {
    return 1;
}
int foo2(int i){
    return i*5;
}
int foo21(int i, double d){
    return i*d;
}
int foo3(int i){
    throw std::string ("void f2()");
}

int main(){

    Future<void>  ft0 = Async(foo0, 5);
    ft0.get();
    Future<void>  ft01 = Async(foo01);
    try{
        ft01.get();
    }catch(const std::string& e){
        int i =1;
    }
    ////
    std::future<int>  ft10 = std::async(foo1);
    int i10 = ft10.get();
    Future<int>  ft11 = Async(foo1);
    int i11 = ft11.get();
    ///
    std::future<int>  ft20 = std::async(foo2, 5);
    int i20 = ft20.get();
    Future<int>  ft21 = Async(foo2,5);
    int i21 = ft21.get();
    std::future<int>  ft200 = std::async(foo21, 5, 6.0);
    int i200 = ft200.get();
    Future<int>  ft211 = Async(foo21,5, 6.0);
    int i211 = ft211.get();
    ///
    Future<int>  ft31 = Async(foo3,5);
    try{
        int i31 = ft31.get();
    }catch(const std::string& e){
      int i =1;
    }
    int j= 0;

    return 0;
}





