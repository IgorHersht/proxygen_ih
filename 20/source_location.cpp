#include <map>
#include <assert.h>
#include<string>
#include <source_location>

std::string log(const std::string& severity, const std::string& msg, std::source_location sl = std::source_location::current()) {
    std::string out(severity); out += " ";
    out += sl.file_name(); out += ":";
    out += sl.function_name(); out += ":";
    out += std::to_string(sl.line()); out += "-";
   // out += std::to_string(sl.column()); out += " ";
    out += msg;
    return out;
}
//test
struct C{
    void foo(){
        std::string msg = log("ERROR", "Some error") ;
        //"ERROR /root/work/projects/test/main.cpp:void C::foo():18-Some error"
        int i =1;
    }
};
int main(){

    C c;
    c.foo();

}

