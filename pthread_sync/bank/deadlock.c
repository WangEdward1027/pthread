#include <func.h>

//各自申请自己的锁，保持并请求,导致死锁

typedef struct {
    int id;
    char name[25];
    int balance;
    // 细粒度锁
    pthread_mutex_t mutex;
} Account;

Account acct1 = {1, "xixi", 1000, PTHREAD_MUTEX_INITIALIZER};
Account acct2 = {2, "peanut", 100, PTHREAD_MUTEX_INITIALIZER};

int transfer(Account* acctA, Account* acctB, int money) {
    pthread_mutex_lock(&acctA->mutex);
    sleep(1);  //增加坏的调度的概率
    pthread_mutex_lock(&acctB->mutex);

    if (acctA->balance < money) {
        pthread_mutex_unlock(&acctA->mutex);
        pthread_mutex_unlock(&acctB->mutex);

        return 0;
    }

    acctA->balance -= money;
    acctB->balance += money;

    pthread_mutex_unlock(&acctA->mutex);
    pthread_mutex_unlock(&acctB->mutex);

    return money;
}

void* start_routine1(void* args) {
    int money = (int) args;
    int ret = transfer(&acct1, &acct2, money);
    printf("%s -> %s: %d\n", acct1.name, acct2.name, ret);
    return NULL;
}

void* start_routine2(void* args) {
    int money = (int) args;
    int ret = transfer(&acct2, &acct1, money);
    printf("%s -> %s: %d\n", acct2.name, acct1.name, ret);
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t tid1, tid2;

    pthread_create(&tid1, NULL, start_routine1, (void*)900);
    pthread_create(&tid2, NULL, start_routine2, (void*)100);

    // 主线程等待子线程结束
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    printf("%s: balance = %d\n", acct1.name, acct1.balance);
    printf("%s: balance = %d\n", acct2.name, acct2.balance);
    return 0;
}
