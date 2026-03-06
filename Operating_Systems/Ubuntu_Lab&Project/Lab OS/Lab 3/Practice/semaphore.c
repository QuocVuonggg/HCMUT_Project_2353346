#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_COUNT 1000000
int count;
sem_t sem;

void *f_count(void *sid) {
    int i;
    for (i = 0; i < MAX_COUNT; i++) {
        sem_wait(&sem);
        count++;
        sem_post(&sem);
    }
    printf("Thread %s: holding %d\n", (char *) sid, count);
    return NULL;
}

void *f_sub(void *sid) {
    int i;
    for (i = 0; i < MAX_COUNT; i++) {
        sem_wait(&sem);
        count--;
        sem_post(&sem);
    }
    printf("Thread %s: holding %d\n", (char *) sid, count);
    return NULL;
}

int main(int argc, char* argv[])
{
    printf("Hello world\n");
    pthread_t thread1, thread2;

    count = 0;

    sem_init(&sem, 0, 1);

    pthread_create(&thread1, NULL, &f_count, "1");
    pthread_create(&thread2, NULL, &f_sub, "2");

    pthread_join(thread2, NULL);
    pthread_join(thread1, NULL);
    
    getc(stdin);
}
