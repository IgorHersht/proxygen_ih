
#include <atomic>
#include <string>
#include <iostream>


template<typename T>
class stack
{
	struct node
	{
	    T m_data;
	    node* next{nullptr};
	    node(const T& data) : m_data(data) {}
	    node(T&& data) : m_data(std::move(data)) {}
	};

    std::atomic<node*> m_head{nullptr};
    void push(node*& new_node){
    	  // now make new_node the new head, but if the head
		  // is no longer what's stored in new_node->next
		  // (some other thread must have inserted a node just now)
		  // then put that new head into new_node->next and try again
		  /*
		  bool compare_exchange_strong (T& expected, T desired)
		  {
		  if (this->load() == expected) {
		  this->store(desired);
		  return true;
		  }
		  else {
		  expected = this->load();
		  return false;
		  }
		  }
		  */
    	 while(!m_head.compare_exchange_weak(new_node->next, new_node));
    }
 public:
   // stack():m_head{nullptr}{}//on old compiler
    void push(const T& data){
      node* new_node = new node(data);
      new_node->next = m_head.load();
      push(new_node);

    }

    void push(T&& data){
    	T t = data;
	  node* new_node = new node(std::move(data));
	  new_node->next = m_head.load();
	  push(new_node);

	}

    void print(){// not thread safe
    		for(node* pos = m_head.load(); pos != nullptr; pos = pos->next  ){
    			std::cout << pos ->m_data<<std::endl;
    		}
    	}
};
class Data{
int i;
std::string s;
};
int main()
{
	std::cout <<"OK"<<std::endl;
	stack<std::string> s;

    std::string s2("2");
    std::string s3("3");

    s.push("1");// call  push(T&& data)
    s.push(s2);
    s.push(std::move(s3));
    s.print();

    stack<int> si; // call  push(T&& data)
    si.push(25);
   
 stack<int> sD; // call  push(T&& data)
	Data d;
    si.push(std::move(d));


    return 0;
}

