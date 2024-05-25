#include <func.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // 静态初始化：默认属性
// 状态：未初始化，初始化，上锁，没上锁，销毁..

void* start_routine(void* args) {
    long* value = (long*) args;
    for(long i = 0; i < 100000000; i++) {
        pthread_mutex_lock(&mutex);
        (*value)++;   // 临界区：对共享资源的操作
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main(int argc, char* argv[])
{
    long* value = (long*)calloc(1, sizeof(long)); // *value = 0
    
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, start_routine, value);
    pthread_create(&tid2, NULL, start_routine, value);

    // 主线程等待两个子线程结束
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // 销毁互斥锁
    pthread_mutex_destroy(&mutex);

    printf("value = %ld\n", *value);
    return 0;
}
