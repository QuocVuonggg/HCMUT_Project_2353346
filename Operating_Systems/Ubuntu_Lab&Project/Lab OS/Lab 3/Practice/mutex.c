#include <stdio.h>
#include <pthread.h>
#include <time.h>
#define MAX_COUNT 1000000
int count; //data
int register1;
int register2;
pthread_mutex_t lock;
void *f_count(void *sid) {
 int i;
 for (i = 0; i < MAX_COUNT; i++) {
 //race condition
  pthread_mutex_lock(&lock);
  count++;
  pthread_mutex_unlock(&lock);
 }
 printf("Thread %s: holding %d\n", (char *) sid, count);
 
}

void *f_sub(void *sid) {
  int i;
 for (i = 0; i < MAX_COUNT; i++) {
  pthread_mutex_lock(&lock);
  count--;
  pthread_mutex_unlock(&lock);
 }
 printf("Thread %s: holding %d\n", (char *) sid, count);
 
}

int main(int argc, char* argv[])
{
 printf("Hello world\n");
 pthread_t thread1, thread2;
 struct timespec start, end;
 double time_spent;
 count = 0;
 pthread_mutex_init(&lock, NULL);
 
 clock_gettime(CLOCK_MONOTONIC, &start);

 pthread_create(&thread1, NULL, &f_count, "1");
 pthread_create(&thread2, NULL, &f_sub, "2");

 pthread_join(thread2, NULL);
 pthread_join(thread1, NULL);
 clock_gettime(CLOCK_MONOTONIC, &end);
 time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
                 
 printf("Time: %f seconds\n", time_spent);
    
 getc(stdin);
}

