#include      <string>
#include      <cstring>
#include      <cstdint>
#include      <algorithm>
#include      <type_traits>
#include      <array>


#if(  __SIZEOF_INT128__  == 16)
        using int128_t =  __int128_t;
        using uint128_t =  __uint128_t;
#else
        using int128_t =  std::array<std::int64_t, 2>;
        using uint128_t = std::array<std::uint64_t, 2>;
#endif
namespace PodStringImpl{    
    template<std::size_t MaxSize, bool enabled = false> struct StorageTypeTrait;

    template<std::size_t MaxSize> struct StorageTypeTrait< MaxSize, (MaxSize < 8 ) >{  
		using ImplT = std::uint8_t;
	   static constexpr  bool IsArithmetic() {return true;}
	};
    
     template<std::size_t MaxSize, typename T> 
     struct StorageTypeTrait< MaxSize, T, typename std::enable_if<
                                    std::is_pod<T>::value &&
                                   ( MaxSize >= sizeof(T) )
                                >::type
                              >
    {         
             static constexpr std::size_t  MinStorageSize(){ return  MaxSize +1 ; }
             static constexpr std::size_t Length(){                
                 return (( MinStorageSize() % sizeof(T)) > 0 ) ? (( MinStorageSize() / sizeof(T))  +1 ) : ( MinStorageSize() / sizeof(T));
                 
             }
            using ImplT = std::array<T,  Length() >;
            static constexpr  bool IsArithmetic() {return false ;}
            static constexpr std::size_t  Alignment(){ return  alignof(ImplT) ; }
  
    };

}

template<std::size_t MaxSize, typename T = std::uint64_t>  struct PodString {
 
 using storage_type = PodStringImpl::StorageTypeTrait<MaxSize, T>;    
 using value_type = typename storage_type::ImplT; 
 static_assert(MaxSize < sizeof(value_type), " Coding error. maxSize >= sizeof(value_type)" );
 
 PodString(value_type value)  noexcept  : m_value{value} {}
 PodString(const  char* value, std::size_t len) noexcept{
  if(len < sizeof(value_type)) {
                memcpy(&m_value, value, len); 
        }
  }

 PodString(const  char* c_str) noexcept : PodString(c_str,  strlen(c_str)  ) { }
 PodString(const  std::string &str) noexcept : PodString(str.c_str(), str.length()){ }

 bool IsValid() const noexcept{
        return ( m_value != Invalid()  );
  }
 
 operator  bool()  const noexcept {
        return   IsValid();
 }

 operator  value_type()  const noexcept {
        return  m_value;
 }
 operator  const char* () const noexcept {
  return reinterpret_cast<const char *>  (&m_value) ;
 }
 operator std::string () const noexcept {
       return reinterpret_cast<const char *>  (&m_value) ;
 }
private:
  static constexpr value_type  NullValue()  { return {} ;  }
  static constexpr value_type  Invalid() { return NullValue(); }
private:
  value_type  m_value{ };
  
};

using String_7 = PodString<7> ;
using String_15 = PodString<15, uint128_t> ;
using String_23 = PodString<23> ;


int main()
{

 String_7 value1 ("SPY", 3);
int ll = String_7::storage_type::Length();
//int ll2 = String_7::storage_type::Length();
 std::string v11 = value1;
  const char* v12 = value1;  
  std::uint64_t v13 = value1;

  String_7 value2 ("SPY");
  String_7 value3 (std::string("SPY"));
  
  bool b1 = value1.IsValid(); bool b2 = value2.IsValid(); bool b3 = value3.IsValid();
  std::string v21=value2;
  std::string v31=value3;
  PodStringImpl::StorageTypeTrait<15, uint128_t >::ImplT t;

  String_15 value6("123456789");
 std::string v6 = value6;
  String_15 value7("12345678123456789");
 bool b6 = value6.IsValid();
bool b7 = value7.IsValid();

  String_15 value8("1"); String_15 value9("3");
  String_15::value_type  iml = value8;
  String_15::value_type im2 = value9;
  bool con12 = (iml > im2);
  auto im3 = iml + im2;
  // String_127_agg val10("123456789");
  // int ll = String_127_agg::value_type:: Length();

 std::size_t ll2 = String_23::storage_type::Length();
String_23 value23("12345678123456789");
std::string v23 = value23;
 std::size_t ll3 = ll2;
 bool b23 = value23;
 return 0;
}