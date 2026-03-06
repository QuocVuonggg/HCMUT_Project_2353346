#include <stdio.h>
#include <pthread.h>

#define MAX_COUNT 1000000
int count;
pthread_rwlock_t lock;

void *f_count(void *sid) {
    int i;
    for (i = 0; i < MAX_COUNT; i++) {
        pthread_rwlock_wrlock(&lock);
        count++;
        pthread_rwlock_unlock(&lock);
    }
    printf("Thread %s: holding %d\n", (char *) sid, count);
    getc(stdin);
}

void *f_sub(void *sid) {
    int i;
    for (i = 0; i < MAX_COUNT; i++) {
        pthread_rwlock_wrlock(&lock);
        count--;
        pthread_rwlock_unlock(&lock);
    }
    printf("Thread %s: holding %d\n", (char *) sid, count);
    getc(stdin);
}

int main(int argc, char* argv[])
{
    printf("Hello world\n");
    pthread_t thread1, thread2;

    count = 0;

    pthread_rwlock_init(&lock, NULL);

    pthread_create(&thread1, NULL, &f_count, "1");
    pthread_create(&thread2, NULL, &f_sub, "2");

    pthread_join(thread2, NULL);

    pthread_join(thread1, NULL);
    
    getc(stdin);
}
