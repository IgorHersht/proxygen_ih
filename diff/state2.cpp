
#include        <cstdint>
#include        <atomic>
#include        <tuple>
#include      <type_traits>


template <typename T > struct StateCode{

  using value_type = T;
  static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value, "This class defined only for unsigned integral types.");
  explicit StateCode(T value): m_value(value){}
   T Load()const noexcept{
       return m_value.load();
    }  
  void Store(T state ) noexcept{
        m_value.store(state);
    }
   bool IsSet(T state)const noexcept{
       auto temp = (Load() & state);
       return (temp == state);
    } 
   void Set(T state ) noexcept{
        m_value.fetch_or(state);
    }
   void UnSet(T state ) noexcept{
       m_value.fetch_and(~state);
    }
 private:
    std::atomic<T> m_value ;
};


 
 struct ConfigStateCode : public StateCode< std::uint8_t> 
 {
     enum  StateId : value_type {ERROR = (1 << 1), NOT_INIT = (1 << 2) };
      enum{ID = 0 };
     
     ConfigStateCode(): StateCode< value_type>((value_type)NOT_INIT){}
     bool IsOk()const noexcept {
         return (Load() == (value_type)0);
     }
 };
 
 struct LogStateCode : public StateCode< std::uint8_t> 
 {
     enum  StateId : value_type {ERROR = (1 << 1), NOT_INIT = (1 << 2) };
      enum{ID = 1 };
      
     LogStateCode(): StateCode< value_type>((value_type)NOT_INIT){}
     bool IsOk()const noexcept {
         return (Load() == (value_type)0);
     }
 };
  
struct SystemState{
    using value_type = std::tuple< ConfigStateCode, LogStateCode>;
    
    template <typename T > T &  Get()  noexcept   {
        static_assert(std::tuple_element<T::ID,value_type> ::type::ID == T::ID,"Code error ");
        return std::get<T::ID>(m_value);
    }
private:  
     value_type m_value;
};



int main(int, char*[])
{
 SystemState state;
 ConfigStateCode &csc = state.Get<ConfigStateCode >();
 int s0 = state.Get<ConfigStateCode >().Load();
 csc.Set(ConfigStateCode::ERROR);
 int s1 = state.Get<ConfigStateCode >().Load();
 bool isOk = state.Get<ConfigStateCode >().IsOk();
 bool isNotInit =state.Get<ConfigStateCode >().IsSet(ConfigStateCode::NOT_INIT);
 state.Get<ConfigStateCode >().UnSet(ConfigStateCode::NOT_INIT);
 int s2 = state.Get<ConfigStateCode >().Load();

 StateCode<std::uint8_t> cs(4);
 cs.Set(2);

    return 0;
}