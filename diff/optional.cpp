
#include <boost/optional/optional.hpp>
#include <iostream>

boost::optional<char> get_input(char ch)
{
    if ( ch != ' ')
        return boost::optional<char>(ch);
    else return boost::optional<char>(); // uninitialized
}


int main()
{
  boost::optional<char> out = get_input('c');
  char ch = 'x';
  if(out){
      ch = *out;
  }
    out = get_input(' ');
    if(out){
         ch = *out;
     }

   return 0;
}

