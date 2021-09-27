

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include  <memory>
#include  <cstdio>

 std::shared_ptr<int> gpi(new int[10], [](int *pi){ delete[] pi;} );

 struct FileDeleter{
   FileDeleter(const std::string &name):m_name(name){
   }
   void operator()(std::ofstream *fp){
     try{
       fp->close();
       std::remove(m_name.c_str());
     }catch (...){
       std::cerr<<"close error.";
     }
   }
 private:
   std::string m_name;
 };

std::shared_ptr<std::ofstream> makeStream(const std::string &fname){
   return std::shared_ptr<std::ofstream>(new std::ofstream(fname),
        [&fname](std::ofstream *fp){
              fp->close();
              std::remove(fname.c_str());
              delete fp;
         }
  );
}


int main()
{

  {
    std::shared_ptr<std::ofstream> st = makeStream("temp.txt");

    int i = 1;
  }

 
  return 0;
}

