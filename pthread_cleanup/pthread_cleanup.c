#include <func.h>

//线程清理函数:pthread_cleanup_push、pthread_cleanup_pop

void cleanup(void* args) {
    char* msg = (char*) args;
    printf("cleanup: %s\n", msg);
}

void* start_routine(void* args) {
    // 注册线程清理函数
    pthread_cleanup_push(cleanup, "first");
    pthread_cleanup_push(cleanup, "second");
    pthread_cleanup_push(cleanup, "third");
    
    // pthread_cleanup_pop(1);
    // pthread_cleanup_pop(0);

    sleep(2);
    
    printf("thread1: I'm going to die...\n");
    // pthread_exit(NULL);     // 子线程退出
    // return NULL;
    for (;;) {
        pthread_testcancel();
    }
    // 后面的代码肯定不会被执行

    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
}

int main(int argc, char* argv[])
{
    pthread_t tid;

    int err;
    err = pthread_create(&tid, NULL, start_routine, NULL);
    if (err) {
        error(1, err, "pthread_create");
    }

    sleep(3);
    pthread_cancel(tid); // 给tid发送取消请求

    // 等待子线程结束
    err = pthread_join(tid, NULL);
    if (err) {
        error(1, err, "pthread_join %lu", tid);
    }
    return 0;
}

