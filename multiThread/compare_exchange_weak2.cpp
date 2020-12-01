#include <atomic>

template<typename T>
class lock_free_stack
{
private:
    struct node
    {
        T data;
        node* next;
        node(T const& data_):
            data(data_)
        {}
    };
    std::atomic<node*> head;
public:
    void push(T const& data)
    {
        node* const new_node=new node(data);
        new_node->next=head.load();
        while(!head.compare_exchange_weak(new_node->next,new_node));
    }
};

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
