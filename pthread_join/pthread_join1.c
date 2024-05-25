#include <func.h>

//主线程用pthread_join()接收子线程的返回值

typedef struct{
    int left;
    int right;
} Section;

void* start_routine(void* args){
    Section* sec = (Section*)args;
    int sum = 0;
    for(int i = sec->left; i <= sec->right; i++){
        sum += i;
    }
    return (void*)sum;
}

int main(void) {
    long long n;
    printf("Please input a long long int: ");
    scanf("%lld", &n);

    Section sec1 = {1, n/2};
    Section sec2 = {n/2+1, n};
    
    // 创建两个线程
    // 第一个线程对区间[1,n/2]累加
    // 第二个线程对区间(n/2, n]累加
    pthread_t tid1;
    pthread_t tid2;
    int err = pthread_create(&tid1, NULL, start_routine, &sec1);
    if(err){
        error(1, err, "pthread_create tid1");
    }
    err = pthread_create(&tid2, NULL, start_routine, &sec2);
    if(err){
        error(1, err, "pthread_create tid2");
    }

    // 主线程等待两个子线程结束，并接收它们计算的结果。
    int result1;
    err = pthread_join(tid1, (void**)&result1);
    if(err){
        error(1, err, "pthread_join %lu\n", tid1);
    }

    int result2;
    err = pthread_join(tid2, (void**)&result2);
    if(err){
        error(1, err, "pthread_join %lu\n", tid2);
    }
    
    // 输出最终结果。
    printf("main: sum = %d\n", result1 + result2);
    return 0;
}
