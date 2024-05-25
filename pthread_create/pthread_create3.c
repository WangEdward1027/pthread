#include <func.h>

//子线程的创建:在子线程访问主线程栈里面的数据

typedef struct{
    int id;
    char name[25];
    char gender;
    int chinese;
    int math;
    int english;
} Student;

void print_ids(const char* prefix){
    printf("%s: ",prefix);
    printf("pid = %d, ppid = %d, tid = %lu\n", getpid(), getppid(), pthread_self());
}

void* start_routine(void* args){
    Student* p = (Student*)args;
    //在子线程里访问主线程栈上面的数据
    printf("%d %s %c %d %d %d\n", p->id,p->name,p->gender,p->chinese,p->math,p->english);

    print_ids("new_thread");
    
    return NULL;
}

int main(int argc, char* argv[])
{
    //主线程
    print_ids("main");
    
    Student s = {1 , "xixi", 'f', 100, 100, 100}; //主线程的栈数据

    pthread_t tid;
    int err = pthread_create(&tid, NULL, start_routine, &s);
    if(err){
        error(1, err, "pthread_create");
    }

    printf("main: new_thread = %lu\n", tid);
    
    //注意事项: 当主线程终止时,整个进程就终止了。
    sleep(1);  //主线程等待子线程执行完毕。

    return 0;
}
