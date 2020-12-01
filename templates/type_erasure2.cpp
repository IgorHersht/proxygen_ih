
#include <cassert>
#include <memory>
#include <iostream>
#include <boost/concept_check.hpp>
#include <boost/type_erasure/any.hpp>
#include <boost/type_erasure/member.hpp>
#include <boost/type_erasure/free.hpp>
#include <boost/type_erasure/builtin.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/tti/tti.hpp>

template <typename H>
struct HolderConcept : boost::Assignable<H>
                     , boost::CopyConstructible<H>
{
  BOOST_CONCEPT_USAGE(HolderConcept)
  {
    h.store(i);
    i = load(h);
  }

private:
    H h;
  int i;
};

struct SomeHolder
{
  int val = 0;
  void store(int i) { val = i; }
};
int load(SomeHolder& h) { return h.val; }

struct SomeHolder2
{
  int val = 0;
  void store(int i) { val = i; }
  void f() {  }

};
int load(SomeHolder2& h) { return h.val; }

BOOST_CONCEPT_ASSERT((HolderConcept<SomeHolder>));

// Type erased interface
BOOST_TYPE_ERASURE_MEMBER((has_member_store), store, 1)
BOOST_TYPE_ERASURE_FREE((has_free_load), load, 1)

namespace te = boost::type_erasure;

using Holder = boost::mpl::vector<
  te::copy_constructible<>,
  has_member_store<void(int)>,
  has_free_load<int(te::_self&)>,
  te::relaxed
>;

using AnyHolder = te::any<Holder>;
using AnyHolderPtr = std::shared_ptr<AnyHolder>;

void foo(AnyHolderPtr ptr)
{
 ptr->store(2);
 std::cout << load(*ptr);

}

BOOST_CONCEPT_ASSERT((HolderConcept<AnyHolder>));

int main()
{
  AnyHolder h {SomeHolder{}};
  h.store(2);
  int i = load(h);

  AnyHolder h2 {SomeHolder2{}};
    h2.store(2);
    int i2 = load(h2);
    SomeHolder2 &sh2 = (SomeHolder2&)h2;
    sh2.f();
    AnyHolderPtr ptr(std::make_shared<AnyHolder>(SomeHolder2()));
    foo(ptr);

  assert (i == 2);
}
