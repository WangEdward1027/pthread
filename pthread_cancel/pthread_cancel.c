#include <func.h>

void* start_routine(void* args) {
    // 将取消状态设置为 PTHREAD_CANCEL_DISABLE
    // int oldstate;
    // pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldstate);

    // 将取消类型设置为 PTHREAD_CANCEL_ASYNCHROUS
    // int oldtype;
    // pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);

    for(;;) {
      // 设置取消点  
      pthread_testcancel();
      printf("Hello world\n");
      // sleep(5);
    }
}

int main(int argc, char* argv[])
{
    pthread_t tid;

    int err;
    err = pthread_create(&tid, NULL, start_routine, NULL);
    if (err) {
        error(1, err, "pthread_create");
    }

    sleep(1);

    err = pthread_cancel(tid);
    if (err) {
        error(1, err, "pthread_cancel %lu", tid);
    }

    // 等待子线程结束
    err = pthread_join(tid, NULL);
    if (err) {
        error(1, err, "pthread_join %lu", tid);
    }
    return 0;
}

