#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int empty(struct queue_t *q)
{
    if (q == NULL)
        return 1;
    return (q->size == 0);
}

void enqueue(struct queue_t *q, struct pcb_t *proc)
{
    /* TODO: put a new process to queue [q] */
    if (!q || !proc || q->size >= MAX_QUEUE_SIZE) {
        return;
    }
    q->proc[q->size++] = proc;
}

struct pcb_t *dequeue(struct queue_t *q)
{
    /* TODO: return a pcb whose prioprity is the highest
     * in the queue [q] and remember to remove it from q
     * */
    
    if (!q || q->size == 0) {
        return NULL;
    }

    struct pcb_t *first_item = q->proc[0];

    for (int idx = 0; idx < q->size - 1; idx++) {
        q->proc[idx] = q->proc[idx + 1];
    }

    q->size--;

    return first_item;
}

struct pcb_t *purgequeue(struct queue_t *q, struct pcb_t *proc)
{
    /* TODO: remove a specific item from queue
     * */
    if (!q || !proc) return NULL;

    int found_idx = -1;

    for (int i = 0; i < q->size; i++) {
        if (q->proc[i] == proc) {
            found_idx = i;
            break;
        }
    }

    if (found_idx != -1) {
        for (int i = found_idx; i < q->size - 1; i++) {
            q->proc[i] = q->proc[i+1];
        }
        q->size--;
        return proc;
    }

    return NULL;
}
