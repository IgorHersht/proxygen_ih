#include <vector>
#include <thread>
#include <atomic>
#include <boost/lockfree/spsc_queue.hpp>

template <typename T, size_t SIZE = 8192> struct HybridQueue{
    HybridQueue(size_t max_bad_push, size_t max_bad_pop ):
        _max_bad_push(max_bad_push),_max_bad_pop(max_bad_pop){

    }

    template <typename ConstIterator> void push(ConstIterator begin, ConstIterator end){
        size_t available = 0;
        for(size_t tr=0; tr <_max_bad_push; ++tr ){
            available = _queue.write_available();
            if(available){ break;}
        }
        if(available == 0){
            std::this_thread::yield();
            push(begin, end);
        }
        ConstIterator newBegin = _queue.push(begin, begin + available );
        push(newBegin, end);
    }

    void push(T const & t){
        size_t tr=0;
        for(; tr <_max_bad_push; ++tr ){
           if(_queue.push(t)){ break;}
        }
        if(tr == _max_bad_push){
            while(!_queue.push(t)){
                std::this_thread::yield();
            }
        }
    }

    void popAllToVector(std::vector<T>& v)
    {
        const size_t available = _queue.read_available();
        for(size_t tr=0; tr <_max_bad_pop; ++tr ){
            available = _queue.read_available();
            if(available){ break;}
        }
        if(available == 0){
            std::this_thread::yield();
            popAllToVector(v);
        }
        // Does not need to clear the vector because it will:
        // 1. replace old valie with new ones in interval[0... available)
        // and erace old value in the rest [available, old_size)
        v.resize(available);
        size_type poped = pop(&(v[0]), available);
        if(poped < v.size()){// should not happened
            v.resize(poped);
        }
    }

    void pop(T & t){
        size_t tr=0;
        for(; tr <_max_bad_pop; ++tr ){
            if(_queue.pop(t)){ break;}
        }
        if(tr == _max_bad_pop)
        {
            while(!_queue.pop(t)){
                std::this_thread::yield();
            }
        }
    }

private:
    boost::lockfree::spsc_queue<T, boost::lockfree::capacity<SIZE>> _queue;
    size_t _max_bad_push{1024};
    size_t _max_bad_pop{1024};
};
/// TODO test
int main() {




    int i =1;


}
