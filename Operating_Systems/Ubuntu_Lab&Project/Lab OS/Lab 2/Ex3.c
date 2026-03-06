#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>
int rcv() {
    int fd = open("abc.fifo.name", O_RDONLY);
    if (fd == -1) {
        exit(1);
    }
    
    char buffer[1024];
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead == -1) {
        exit(1);
    }
    buffer[bytesRead] = '\0'; 
    printf("Received: %s\n", buffer);
    
    close(fd);
    return 0;
}

int snd(const char *message) {
    int fd = open("abc_fifo_name", O_WRONLY);
    if (fd == -1) {
        exit(1);
    }
    ssize_t bytesWritten = write(fd, message, strlen(message));
    if (bytesWritten == -1) {
        exit(1);
    }
    printf("Sent: %s\n", message);
    
    close(fd);
    return 0;
}


int main()
{
    snd("abc_fifo_name");

    rcv("abc_fifo_name");

    return 0;
}

