#include <stdio.h>
#include <iostream>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>


#include <unistd.h>
const char* semName = "asdfsd";
sem_t* sem_id = NULL;
void parent(void) {
    if (sem_id == SEM_FAILED) {
        perror("Parent  : [sem_open] Failed\n"); return;
    }
    printf("Parent  : Wait for Child to Print\n");

    for (int i = 0; i < 3; ++i) {
        if (sem_wait(sem_id) < 0) {
            printf("Parent  : [sem_wait] Failed\n");
        }
        else {
            std::cout << "Parent  : One child posted " << i << std::endl;
        }
    }

    std::cout << "Parent  : All children posted " << std::endl;


}
void child(int i)
{
    std::cout << "Child started " << i << std::endl;
    // sem_t *sem_id = sem_open(semName, O_CREAT, 0666, 0);
    if (sem_id == SEM_FAILED) {
        perror("Child   : [sem_open] Failed\n"); return;
    }
    sleep(5 + i * 5);
    std::cout << "Child  Release Semaphore " << i << std::endl;
    if (sem_post(sem_id) < 0)
        printf("Child   : [sem_post] Failed \n");
    //sem_close(sem_id);
    sleep(5 + i * 5);
    std::cout << "Child  exit " << i << std::endl;
    exit(0);
}



int main(int argc, char* argv[])
{
    sem_id = sem_open(semName, O_CREAT, 0666, 0);
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

    parent();
    if (sem_close(sem_id) != 0) {
        perror("Parent  : [sem_close] Failed\n");
    }
    if (sem_unlink(semName) < 0) {
        printf("Parent  : [sem_unlink] Failed\n");
    }

    for (int i = 0; i < 3; ++i) {
        wait(NULL);
    }

    printf("Parent  : exit  \n");
    return 0;
}