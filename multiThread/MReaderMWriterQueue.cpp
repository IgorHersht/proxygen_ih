#include <atomic>

template<typename T> class  MReaderMWriterQueue{
  struct Node{
    explicit Node(const T& data):_data(data){}
    T _data;
    Node* _next{};
  };
  std::atomic<Node*> _head;
public:
  void push(const T& data){
    Node* nn= new Node(data);
    nn->_next = _head;
    while(!_head.compare_exchange_weak(nn->_next, nn));
  }
  
};

int main()
{


  int i =1;
  return 0;
}
/*
bool compare_exchange_weak (T& expected, T desired)
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
