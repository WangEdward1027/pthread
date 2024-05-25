#include <func.h>

void* start_routine(void* args) {
    long* value = (long*) args;
    for(long i = 0; i < 100000000; i++) {
        (*value)++;
    }
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

    printf("value = %ld\n", *value);
    return 0;
}
