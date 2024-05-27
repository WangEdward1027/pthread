#ifndef __WD_BLOCKQ_H
#define __WD_BLOCKQ_H

#include <func.h>
#define N 1024

//阻塞队列的实现

typedef int E;

// 循环数组实现队列
typedef struct {
    E elements[N];    
    int front;
    int rear;
    int size;

    pthread_mutex_t mutex;
    pthread_cond_t  not_empty;  // 非空
    pthread_cond_t  not_full;   // 不满
} BlockQ;

// API
BlockQ* blockq_create(void);
void blockq_destroy(BlockQ* q);

void blockq_push(BlockQ* q, E val);
E    blockq_pop(BlockQ* q);
E    blockq_peek(BlockQ* q);
bool blockq_empty(BlockQ* q);
bool blockq_full(BlockQ* q);

#endif
