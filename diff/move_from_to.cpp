
#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <random>
#include <iostream>
#include <exception>
#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<list>
#include<vector>
#include<queue>
#include<algorithm>
#include<string>
using namespace std;


struct Data{
    Data(){
       int i =1; 
    }
    Data(const std::string & s): m_data(s){
        int i = 9;
    }
     Data(const Data& d):m_data(d.m_data){
         int i =1; 
    }
      Data(Data&& data): m_data(std::move(data.m_data)){
         int i =1; 
    }
      std::string m_data{"aaaaa"};
};

struct From{
     Data& d()   {
        return m_data;
    }
     From(const Data& d):m_data(d){
        int i =1;  
     }
   
    
    Data m_data;
};

struct To{
    To( Data&& data):m_data(std::move(data)){
         int i =1; 
    }
    To( const Data& data):m_data(data){
         int i =1; 
    }
    To(){
         int i =1; 
    }
    Data m_data;
};


int main(int argc, char* argv[]){
   /*
    *init type To moving data from From
    */
    Data d("bbbbb");
   From f(d); 
   To t ( std::move(f.d()));
   
    
 return 0;
}
 
