#include<string.h>

enum class E1;
void f(E1 e);
enum struct E1{ V1, V2};

void f(E1 e){
	int i = (int)e;
	e = (E1)1;
}

enum Color: std::uint8_t;
void f(Color e);
enum Color: std::uint8_t{
	RED
};
void f(Color e){
	int i = RED;
}

int main(int argc, char* argv[]){

E1 e = E1::V1;

f(E1::V1);

 return 0;
}
