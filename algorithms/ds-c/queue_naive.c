/*
 * iagorrr ;)
 * Naive queue implementation.
 *      - static size.
 *      - 'methods':
 *          - initialize.
 *          - enqueue.
 *          - dequeue.
 *          - front.
 *          - back.
 *          - size.
 *          - full.
 *          - empty.
 */
#include <stdio.h>
#include <stdlib.h>
#define MAXQUEUESIZE (int)1e5
#define Item_t int

typedef struct queue
{
    int l;
    int r;
    int maxsize;
    Item_t *q;
    
    void (*enqueue)(struct queue*, Item_t);
    void (*dequeue)(struct queue*);
    char (*empty)(struct queue*);
    char (*full)(struct queue*);
    Item_t (*front)(struct queue*); 
    Item_t (*back)(struct queue*);
    int (*size)(struct queue*);
    void (*destroy)(struct queue*);
} queue;

void queue_enqueue(queue *q, Item_t i);
void queue_dequeue(queue*q);
Item_t queue_front(queue *q);
Item_t queue_back(queue *q);
char queue_empty(queue *q);
char queue_full(queue* q);
queue queue_initialize(int maxs);
int queue_size(queue *q);
void queue_destroy(queue *q);

int main(void)
{

    queue q = queue_initialize(MAXQUEUESIZE);
    printf("%d %d\n", q.l, q.r);

    q.enqueue(&q, 30);

    printf("%d\n", q.front(&q));
    printf("%d\n", q.back(&q));

    q.enqueue(&q, 40);
    printf("%d\n", q.front(&q));
    printf("%d\n", q.back(&q));

    q.dequeue(&q);
    printf("%d\n", q.front(&q)); 
    printf("%d\n", q.size(&q));

    q.destroy(&q);
}


queue queue_initialize(int maxs)
{
    queue new;

    new.q = malloc(sizeof(Item_t)*maxs);

    new.l = 0;
    new.r = 0;

    new.maxsize = maxs;

    new.enqueue = queue_enqueue;
    new.dequeue = queue_dequeue;
    new.full = queue_full;
    new.empty = queue_empty;
    new.front = queue_front;
    new.back = queue_back;
    new.size = queue_size;
    
    new.destroy = queue_destroy;
    return new;
}

char queue_full(queue* q)
{
    return q->r == q->maxsize;
}

char queue_empty(queue *q)
{
    return q->l == q->r;
}

Item_t queue_back(queue *q)
{
    return q->q[q->r-1];
}

Item_t queue_front(queue *q)
{
    return q->q[q->l];
}

void queue_dequeue(queue*q)
{
    ++q->l;
}

void queue_enqueue(queue *q, Item_t i) {
    if(q->full(q)) return;

    q->q[q->r++] = i;
}

int queue_size(queue* q)
{
    return q->r - q->l;
}

void queue_destroy(queue *q)
{
    free(q->q);
}
