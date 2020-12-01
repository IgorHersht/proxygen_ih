#include <boost/bind.hpp>
#include <boost/threadpool.hpp>

#include <atomic>
/*
 * T expected;
 * atomic(T &expected, T desir){
 *if(atomic == expected){ 
  atomic= desir;
  expected = atomic;
 retur true;
 }
 * ;
 * expected = atomic;
 * return false;
 
 */
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
 std::atomic<node*> m_head;
public:
    void push(T const& data)
    {
        node *nn = new node(data);
        nn->next = m_head.load();
        while(!m_head.compare_exchange_weak(nn->next, nn)){
            continue;
        }
    }
};
lock_free_stack<std::string> ss;
int main()
{
    
    ss.push("aaa"); ss.push("b"); ss.push("ccc");
    
    return 0;
}