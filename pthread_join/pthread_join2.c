#include <func.h>

//pthread_join: 
//注意：子线程不能返回指向自己栈上数据的指针
//因为当子线程退出的时候，子线程的栈会销毁！
//只能返回子线程堆上的数据

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

    // 主线程：等待子线程结束，并接收返回值
    Student* s1;
    err = pthread_join(tid1, (void**)&s1); // 无限期等待
    if (err) {
        error(1, err, "pthread_join %lu\n", tid1);
    }

    print_stu_info(s1);

    free(s1);

    return 0;
}

