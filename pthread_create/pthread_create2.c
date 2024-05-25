#include <func.h>

//子线程的创建: args传参数 int 

void print_ids(const char* prefix){
    printf("%s: ",prefix);
    printf("pid = %d, ppid = %d, tid = %lu\n", getpid(), getppid(), pthread_self());
}

void* start_routine(void* args){
    int i = (int) args;    //C语言是弱类型语言，可强转
    printf("new_thread: i = %d\n",i);
    /* print_ids("thread"); */
    
    return NULL;
}

int main(int argc, char* argv[])
{
    //主线程
    print_ids("main");
    
    pthread_t tid;
    int err = pthread_create(&tid, NULL, start_routine, (void*)1027);
    if(err){
        error(1, err, "pthread_create");
    }

    printf("main: new_thread = %lu\n", tid);
    
    //注意事项: 当主线程终止时,整个进程就终止了。
    sleep(1);  //主线程等待子线程执行完毕。

    return 0;
}
