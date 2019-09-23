#include<iostream>
#include<queue>
#include<pthread.h>
#include<unistd.h>
using namespace std;


//类的成员函数作为线程函数使用
class cs
{
    public:
        static cs * pthis;         //这里有一个隐性的问题　成员函数不能作为回调函数
        friend void * ddd(void *arg);
        static void * customer(void *arg);//可以申请一个静态指针　把对象实体赋给指针　记的静态成员要初始化
        static void * producer(void *arg);//原因是因为类外初始化可保证类内的等于号始终为赋值
        cs();
        cs& operator=(const cs &tmp) = default;
        cs(const cs &tmp) = default;
        ~cs();
    private:
        queue<int>que;
        void tempa(){cout << "消费者已消费！\n";}
        void tempb(){cout << "生产者已生产！\n";}
        pthread_mutex_t mux;
        pthread_cond_t  con;
        pthread_t com_ptr;
        pthread_t pro_ptr;

};

cs::~cs()
{
    pthread_cond_broadcast(NULL);
    pthread_join(com_ptr,NULL);
    pthread_join(pro_ptr,NULL);
    std::cout << "两个线程已销毁\n";
}

cs::cs():com_ptr(0),pro_ptr(0)  //赋值一定要注意　不然容易段错误
{
    pthread_mutex_init(&mux,NULL);
    pthread_cond_init(&con,NULL);
    pthread_create(&com_ptr,NULL,pthis->producer,NULL);
    pthread_create(&pro_ptr,NULL,pthis->customer,NULL);
}

void *ddd(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&(pthis->mux));
        pthis->que.push(1);
        pthis->tempb();
        pthread_cond_signal(&(pthis->con));
        pthread_mutex_unlock(&(pthis->mux));
        sleep(2);
    }
}

void * cs::producer(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&(pthis->mux));
        pthis->que.push(1);
        pthis->tempb();
        pthread_cond_signal(&(pthis->con));
        pthread_mutex_unlock(&(pthis->mux));
        sleep(2);
    }
}

void *cs::customer(void *arg)
{
    cout << "消费者\n";
    while(1)
    {
        pthread_mutex_lock(&(pthis->mux));
        while(pthis->que.empty())
        {
            pthread_cond_wait(&(pthis->con),&(pthis->mux));
        }
        if(pthis->que.empty())
        {
            cerr << "模型出现错误！\n";
            break;  //这个函数只要退出就会发生错误
        }
        pthis->que.pop();
        pthis->tempa();
        pthread_mutex_unlock(&(pthis->mux));
    }
}
cs * cs::pthis=NULL;   //静态成员必须初始化　
int main()
{
    cs tmp;
    cs::pthis = &tmp;  //把对象本身赋值给这个静态指针 //不足就是破坏了类的封装性
    while(1)
    sleep(30);
    return 0;
}