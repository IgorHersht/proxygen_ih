#include <stdio.h>
#include <iostream>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>


#include <unistd.h>
const char* semName = "asdfsd";

struct CountingSemaphore {

    explicit CountingSemaphore(const char* name) : _name(name) {
        _sem = sem_open(name, _oflag, _mode, _value);
        if (_sem == SEM_FAILED) {
            std::cerr << "sem_open failed. errno " << strerror(errno) << std::endl;
            exit(1);
        }
    }

    ~CountingSemaphore() {
        if (sem_close(_sem)) {
            std::cerr << "sem_close failed. errno " << errno << std::endl;
            exit(1);
        }
    }

    void post() {
        if (sem_post(_sem)) {
            std::cerr << "sem_post failed. errno " << strerror(errno) << std::endl;
            exit(1);
        }
    }

    void wait(int count) {
        for (int i = 0; i < count; ++i) {
            if (sem_wait(_sem)) {
                std::cerr << "sem_wait failed. errno " << strerror(errno) << std::endl;
                exit(1);
            }
            int j = 1;
        }
    }

    static void unlink(const char* semName) {
        int status = sem_unlink(semName);
        if (!status) {
            return;
        }
        std::string msg = std::string("Cannot unlink semaphore ") + semName;
        switch (status) {
        case EACCES: {
            msg += ". Permission is denied to unlink the named semaphore. ";
            break;
        }
        case ENAMETOOLONG: {
            msg += ". The length of the name argument exceeds {PATH_MAX} or a pathname component is longer than {NAME_MAX}. ";
            break;
        }
        case ENOENT: {
            msg += ". The named semaphore does not exist. ";
            break;
        }
        default: break;
        }
        std::cerr << msg << std::endl;
        exit(1);
    }
private:
    inline const static int             _oflag{ O_CREAT };
    inline const static mode_t          _mode{ 0666 };
    inline const static unsigned int    _value{ 0 };

    const char* _name;
    sem_t* _sem;
};


void child(int i)
{
    std::cout << "Child started " << i << std::endl;
    {
        CountingSemaphore sem(semName);
        sleep(5 + i * 5);
        sem.post();
        std::cout << "Child  Release Semaphore " << i << std::endl;
    }
    sleep(5 + i * 5);
    std::cout << "Child  exit " << i << std::endl;
    exit(0);
}


int main(int argc, char* argv[])
{
    for (int i = 0; i < 3; ++i) {
        pid_t pid;
        pid = fork();
        if (pid < 0) {
            perror("fork");
        }
        if (!pid) {
            child(i);
        }
        else {

        }

    }

    {
        CountingSemaphore sem(semName);
        printf("Parent  : Wait for Child to post\n");
        sem.wait(3);
    }
    CountingSemaphore::unlink(semName);
    for (int i = 0; i < 3; ++i) {
        wait(NULL);
    }
    printf("Parent  : exit  \n");
    return 0;
}