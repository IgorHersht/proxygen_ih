#include <boost/endian/arithmetic.hpp>
#include <boost/endian/conversion.hpp>
#include <vector>
#include <algorithm>
#include <type_traits>

struct RecordIn
{
  boost::endian::big_uint32_at   id {2};       // big endian
  boost::endian::big_int32_at    balance{-3};  // big endian
};
struct RecordOut
{
  uint32_t   id;       // native ( small) endian
  int32_t    balance; // native ( small) endian
};
alignas(std::alignment_of_v<RecordOut>) std::vector<RecordIn> vecIn(10);
char* buf = (char*)(&(vecIn[0]));

char* last = buf + vecIn.size() * sizeof(RecordIn);

int main()
{
  std::vector<RecordOut> vecOut;
  for(char* pos = buf; pos < last; pos += sizeof(RecordIn)  ){
    RecordOut* ru = (RecordOut*) pos;
    ru->id = boost::endian::big_to_native(ru->id);
    ru->balance = boost::endian::big_to_native(ru->balance);
    vecOut.emplace_back(*ru);
  }
  int i =1;
  return 0;
}
