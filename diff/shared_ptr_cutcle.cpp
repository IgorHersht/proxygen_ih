
#include <memory>

#include <iostream>


class B;

struct A{
	void set (std::shared_ptr<B> b);
	std::shared_ptr<B> m_b;
};


struct B{
	void set(std::shared_ptr<A> a);
	std::shared_ptr<A> m_a;
};

void A::set(std::shared_ptr<B> b){
	m_b = b;
}
void B::set(std::shared_ptr<A> a){
	m_a =a;
}

void ad(A *a){// deleter never executed
	delete a;
}
void bd(B *b){ //deleter never executed
	delete b;
}

int main()
{


	{
		std::shared_ptr<A> a(new A, ad);
		std::shared_ptr<B> b(new B, bd);
		int ac0 = a.use_count();
		int bc0 = b.use_count();
		a->set(b);
		b->set(a);
		int bc1 = b.use_count();
		int ac =  a.use_count();
	}



	return 0;
}


 
