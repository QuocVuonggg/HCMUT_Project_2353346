#include <stdio.h>
#include <pthread.h>

#define MAX_COUNT 10000000;
volatile unsigned int count;

int __inline__ atomicCAS( volatile unsigned int *ptr, int cmp, int new)
{
 
    unsigned char ret;
 
    __asm__ __volatile__ (
    " lock\n"
    " cmpxchgl %2,%1\n"
    " sete %0\n"
    : "=q" (ret), "=m" (*ptr)
    : "r" (new), "m" (*ptr), "a" (cmp)
    : "memory");
 
    return (int) ret;
}

void *f_count(void *sid) {
    int i;
    unsigned int old, new;
    
    for (i = 0; i < MAX_COUNT; ++i) {
        do {
          old = count;
          new = old + 1;
        } while (atomicCAS(&count, old, new) == 0);
    }
    printf("Thread %s: holding %d\n", (char *) sid, count);
    getc(stdin);
}

void *f_sub(void *sid) {
    int i;
    unsigned int old, new;
    
    for (i = 0; i < MAX_COUNT; ++i) {
        do {
          old = count;
          new = old - 1;
        } while (atomicCAS(&count, old, new) == 0);
    }
    printf("Thread %s: holding %d\n", (char *) sid, count);
    getc(stdin);
}

int main(int argc, char* argv[]){
  printf("Hello world\n");
  pthread_t thread1, thread2;
  pthread_create(&thread1, NULL, &f_count, "1");
  pthread_create(&thread2, NULL, &f_sub, "2");
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  getc(stdin);
}
