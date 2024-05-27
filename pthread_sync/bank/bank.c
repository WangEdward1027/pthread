#include <func.h>

//银行取钱的例子，研究同步与并发
//细粒度锁，放在结构体里

typedef struct {
    int id;
    int balance;
    pthread_mutex_t mutex; // 细粒度锁
} Account;

Account acct1 = {1, 100, PTHREAD_MUTEX_INITIALIZER};
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // 全局锁，粒度太大

int withdraw(Account* acct, int money) {
    pthread_mutex_lock(&acct->mutex);
    if (acct->balance < money) {
        pthread_mutex_unlock(&acct->mutex);
        return 0;
    }

    sleep(1);   // 让某种调度出现的概率最大化

    acct->balance -= money;
    pthread_mutex_unlock(&acct->mutex);

    printf("%lu: withdraw %d\n", pthread_self(), money);
    return money;
}

void* start_routine(void* args) {
    withdraw(&acct1, 100);
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, start_routine, NULL);
    pthread_create(&tid2, NULL, start_routine, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    
    // pthread_mutex_destroy(&mutex);
    // 打印账号的余额
    printf("balance: %d\n", acct1.balance);
    return 0;
}
