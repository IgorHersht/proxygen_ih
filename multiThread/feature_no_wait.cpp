#include <future>
#include <iostream>

/*
 * With the async launch policy, you don’t necessarily have to call get() anymore because, if
the lifetime of the returned future ends, the program will wait for func1() to finish
 */
using namespace std;

void foo()
{
  cout << "start foo" << endl;
  this_thread::sleep_for(chrono::seconds(10));
  cout << "end foo" << endl;
}

int main()
{
  async(launch::async, foo);
  return 0;
}
/*
start foo
end foo
 */

