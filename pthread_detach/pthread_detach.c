#include <func.h>

//主线程主动使用pthred_detach,则子线程退出后系统会自动回收其资源,主线程不需要也无法显式地调用pthread_join来回收子进程的返回值

typedef struct {
    int id;
    char name[25];
    char gender;
    int chinese;
    int math;
    int english;
} Student;

void print_stu_info(Student* s) {
    printf("%d %s %c %d %d %d\n",
           s->id,
           s->name,
           s->gender,
           s->chinese,
           s->math,
           s->english);
}

void* start_routine(void* args) {
    // 注意：不能返回指向该线程栈上数据的指针
    // 因为当线程退出的时候，该线程的栈会销毁！
    // Student s = {1, "xixi", 'f', 100, 100, 100};

    Student* s = (Student*) malloc(sizeof(Student));
    s->id = 1;
    strcpy(s->name, "xixi");
    s->gender = 'f';
    s->chinese = 100;
    s->math = 100;
    s->english = 100;

    return (void*) s;
}

int main(int argc, char* argv[])
{
    // 主线程
    pthread_t tid1;

    int err = pthread_create(&tid1, NULL, start_routine, NULL);
    if (err) {
        error(1, err, "pthread_create");
    }

    // 主线程主动调用 pthread_detach
    err = pthread_detach(tid1);   // 使tid1处于游离状态
    if (err) {
        error(1, err, "pthread_detach %lu", tid1);
    }

    // 主线程：等待子线程结束，并接收返回值
    Student* s1;
    
    // 子线程已经处于detached状态,现在主线程使用pthread_join会报错
    // ./pthread_detach: pthread_join 139830164506368: Invalid argument
    err = pthread_join(tid1, (void**)&s1); // 无限期等待
    if (err) {
        error(1, err, "pthread_join %lu", tid1);    
    }

    print_stu_info(s1);

    free(s1);

    return 0;
}

