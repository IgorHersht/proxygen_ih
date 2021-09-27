#include <iostream>
#include<memory>

#include<string>
#include<string.h>


struct Char{

	~Char(){
		//std::cout <<"d-"<< m_ch<<std::flush;
		k++;
	}

char m_ch= 'z';
static int k;
};
int Char::k= 0;

template <typename UPT>
void useUnique( UPT && up){{
		UPT uptemp(std::move(up));
		std::cout <<(char*) uptemp.get() <<std::endl<<std::flush;
	}
	int t =1;
}

template <typename T>
void useUniqueFromHeapArray( T  *ptrIn){
	auto deleter = [](T  *ptr){
		delete [] ptr;
	};
	std::unique_ptr<T, decltype(deleter)> uptemp(ptrIn, deleter);
	useUnique(std::move(uptemp));
}

void test1(){
	const char* temp1 = "data1";
	std::unique_ptr<Char[]> data1(new Char[strlen(temp1) +1]);
	memcpy(data1.get(), temp1, strlen(temp1) +1 );
	useUnique(std::move(data1));
	std::cout << Char::k <<std::endl<<std::flush;
}

void test2(){
	const char* temp2 = "data2";
	Char* data2 = new Char[strlen(temp2) +1];
	memcpy(data2, temp2, strlen(temp2) +1 );
	useUniqueFromHeapArray(data2);
	std::cout << Char::k <<std::endl<<std::flush;
}


int main(int argc, char* argv[]){

test2();

//

 return 0;
}
