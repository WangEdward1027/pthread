#include <func.h>

void print_ids(const char* prefix){
    printf("%s: ",prefix);
    printf("pid = %d, ppid = %d, tid = %lu\n", getpid(), getppid(), pthread_self());
}

//子线程
void* start_routine(void* args){
    print_ids("thread");
    
    return NULL;
}

int main(int argc, char* argv[])
{
    //主线程
    print_ids("main");
    
    pthread_t tid;
    int err = pthread_create(&tid, NULL, start_routine, NULL);
    if(err){
        error(1, err, "pthread_create");
    }

    printf("main: new_thread = %lu\n", tid);
    
    //注意事项: 当主线程终止时,整个进程就终止了。
    sleep(1);  //主线程等待子线程执行完毕。

    return 0;
}
