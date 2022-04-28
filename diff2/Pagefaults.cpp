
#include <sys/mman.h>	// Needed for mlockall()
#include <unistd.h>		// needed for sysconf(int name);
#include <malloc.h>
#include <sys/resource.h>	// needed for getrusage
#include<utility>
#include <cassert>


// Pagefaults, std::pair<major,minor>
using Pagefaults = std::pair<int,int>;
inline Pagefaults newPagefaultCount(){
    static int last_majflt = 0, last_minflt = 0;
    rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    std::pair<int,int> pagefauls{usage.ru_majflt - last_majflt,usage.ru_minflt - last_minflt };
    last_majflt = usage.ru_majflt;
    last_minflt = usage.ru_minflt;

    return pagefauls;
}

inline Pagefaults isStackSafe(size_t stacksize){
    volatile char buffer[stacksize];
    // Touch each page in this piece of stack
    const size_t pageSize = sysconf(_SC_PAGESIZE);
    for (size_t i = 0; i < stacksize; i += pageSize) {
        buffer[i] = i;
    }
    const std::pair<int,int> pagefauls = newPagefaultCount();
    return pagefauls;
}

inline void touchHeap(size_t size) {
    char *buffer = (char*) malloc(size);

    // Touch each page of the heap
    for (size_t i = 0; i < size; i += sysconf(_SC_PAGESIZE)) {
        /* Each write to this buffer will generate a pagefault.
           Once the pagefault is handled a page will be locked by memlock in
           memory and never given back to the system.
       */
        buffer[i] = 0;
    }
    /*
     * buffer will now be released. The memory allocated above is
       locked for this process.  Issuing free() and
       delete() does NOT make mlock locking undone.
     */

    free(buffer);
}

//test
#include<iostream>
constexpr size_t HEAP_SIZE (1024 * 1024 * 1000);
constexpr size_t STACK_SIZE (1024 * 100);

void print(Pagefaults pagefaults){
    std::cout<< "Major-Pagefaults="<< pagefaults.first << " Minor-Pagefaults=" << pagefaults.second << std::endl;
}
int main()
{
    // before mlock
    Pagefaults pf10 =newPagefaultCount();
    Pagefaults pf11 = isStackSafe(STACK_SIZE);
    touchHeap(HEAP_SIZE);
    Pagefaults pf12 = newPagefaultCount();

    // mlock will lock stack and heap
    // assert(!mlockall(MCL_CURRENT | MCL_FUTURE) ){// can go out of memory if a lot of heap allocations in future.
    assert(!mlockall(MCL_CURRENT ));
    Pagefaults pf20 =newPagefaultCount();
    Pagefaults pf21 = isStackSafe(STACK_SIZE);
    touchHeap(HEAP_SIZE);
    Pagefaults pf22 = newPagefaultCount();
    //
    munlockall();
    ///
    std::cout << "page size=" << sysconf(_SC_PAGESIZE)<< " bytes" << std::endl;
    print(pf10);
    print(pf11);
    print(pf12);

    print(pf20);
    print(pf21);
    print(pf22);

    return 0;
}
/*
page size=4096
Major-Pagefaults=0 Minor-Pagefaults=546
Major-Pagefaults=0 Minor-Pagefaults=25
Major-Pagefaults=0 Minor-Pagefaults=1032
Major-Pagefaults=0 Minor-Pagefaults=806
Major-Pagefaults=0 Minor-Pagefaults=0
Major-Pagefaults=0 Minor-Pagefaults=1024
 *
 */


