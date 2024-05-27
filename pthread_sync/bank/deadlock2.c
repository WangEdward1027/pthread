#include <func.h>

//破坏死锁的必要条件2: 不剥夺
//获取锁1后，尝试获取锁2，若失败则立刻放弃已持有的锁1

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

    // 2.不能抢占
start:
    pthread_mutex_lock(&acctA->mutex);
    sleep(1);   //增大坏的调度发生的概率。不能在这里随机睡，因为手里拿着锁
    int err = pthread_mutex_trylock(&acctB->mutex);
    if (err) {
        // 主动释放获取的锁
        pthread_mutex_unlock(&acctA->mutex);
        int seconds = rand() % 3;    //注意要随机睡眠一下，保证能错位尝试获取锁
        sleep(seconds);              //刚获取锁失败,释放锁后睡一下,相当于重新排队申请
        goto start;
    }

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
    srand(time(NULL));    //main中设置随机种子

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
