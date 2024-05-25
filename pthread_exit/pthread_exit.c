#include <func.h>

//子线程使用pthread_exit()退出后, start_routine的后续代码不执行

void print_ids(const char* prefix){
    printf("%s: ",prefix);
    printf("pid = %d, ppid = %d, tid = %lu\n", getpid(), getppid(), pthread_self());
}

//子线程
void* start_routine(void* args){
    print_ids("thread");
    
    pthread_exit(NULL);  //子线程终止,主动退出

    printf("new_thread: You Can not see me!\n");
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
