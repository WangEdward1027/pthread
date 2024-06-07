#include "BlockQ.h"

// 创建空的阻塞队列
BlockQ* blockq_create(void) {
    BlockQ* q = (BlockQ*) calloc(1, sizeof(BlockQ));
    
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);

    return q;
}

void blockq_destroy(BlockQ* q) {
    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->not_empty);
    pthread_cond_destroy(&q->not_full);

    free(q);
}

void blockq_push(BlockQ* q, E val) {
    // 获取锁
    pthread_mutex_lock(&q->mutex);
    // 注意事项：一定要写成 while 
    while (q->size == N) {
        // 1. 释放q->mutex
        // 2. 陷入阻塞状态
        // 3. 当pthread_cond_wait返回时，一定再一次获取了q->mutex
        // 语义：返回时，条件曾经成立过，现在是否成立，不确定；需要再一次检查
        // 存在虚假唤醒现象
        pthread_cond_wait(&q->not_full, &q->mutex);
    } // a. 获取了q->mutex; b. q->size != N

    q->elements[q->rear] = val;
    q->rear = (q->rear + 1) % N;
    q->size++;
    // not_empty条件成立, 唤醒等待not_empty条件的线程
    pthread_cond_signal(&q->not_empty);

    pthread_mutex_unlock(&q->mutex);
}

E blockq_pop(BlockQ* q) {
    pthread_mutex_lock(&q->mutex);
    while (q->size == 0) {
        pthread_cond_wait(&q->not_empty, &q->mutex);
    }
    // a. 获取了q->mutex; b. q->size != 0
    E retval = q->elements[q->front];
    q->front = (q->front + 1) % N;
    q->size--;
    
    // not_full条件成立，唤醒等待not_full条件的线程
    pthread_cond_signal(&q->not_full);

    pthread_mutex_unlock(&q->mutex);

    return retval;
}

E blockq_peek(BlockQ* q) {
    pthread_mutex_lock(&q->mutex);
    while (q->size == 0) {
        pthread_cond_wait(&q->not_empty, &q->mutex);
    }
    // a. 获取了q->mutex; b. q->size != 0;
    E retval = q->elements[q->front];
    pthread_mutex_unlock(&q->mutex);
    return retval;
}

bool blockq_full(BlockQ* q) {
    pthread_mutex_lock(&q->mutex);
    int size = q->size;
    pthread_mutex_unlock(&q->mutex);
    return size == N;
}

bool blockq_empty(BlockQ* q) {
    pthread_mutex_lock(&q->mutex);
    int size = q->size;
    pthread_mutex_unlock(&q->mutex);
    return size == 0;
}
