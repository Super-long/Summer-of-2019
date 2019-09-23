#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<vector>
#include<iostream>
using namespace std;

vector<int> vec;
pthread_mutex_t mutex_product; //维护生产者队列
pthread_mutex_t mutex_consume; //维护消费者队列
pthread_cond_t cond;

void* product(void *)      //生产者
{
    int count=0;
    while(1)
    {
        pthread_mutex_lock(&mutex_product);
        vec.emplace_back(count++);
        cout << "生产者进程号：" << pthread_self() << endl;
        cout << "生产者产出: " << count << endl;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex_product);
        sleep(3);
    }
}

void* consume(void *)
{
    while(1){
        pthread_mutex_lock(&mutex_consume);
        while(vec.empty()) //防止虚假唤醒
        {
            pthread_cond_wait(&cond,&mutex_consume);//等待生产折发出信号
        }
        cout << "消费者进程号：" << pthread_self() << endl;
        cout << "消费者得到锁,数据为 ：" << vec.back() << endl;
        vec.pop_back();
        pthread_mutex_unlock(&mutex_consume);
    }
}

int main()
{
    pthread_mutex_init(&mutex_consume,NULL);
    pthread_mutex_init(&mutex_product,NULL);
    pthread_cond_init(&cond,NULL);
    pthread_t pid1,pid2,pid3,pid4;

    //生产者队列
    pthread_create(&pid1,NULL,product,NULL);
    pthread_create(&pid2,NULL,product,NULL);

    //消费者队列
    pthread_create(&pid3,NULL,consume,NULL);
    pthread_create(&pid4,NULL,consume,NULL);
    
    pthread_join(pid1,NULL);
    pthread_join(pid2,NULL);
    pthread_join(pid3,NULL);
    pthread_join(pid4,NULL);

    pthread_mutex_destroy(&mutex_consume);
    pthread_mutex_destroy(&mutex_product);
    pthread_cond_destroy(&cond);
    return 0;
}