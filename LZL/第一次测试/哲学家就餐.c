#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
pthread_mutex_t chopstick[6] ;
void get(int *left,int *right,char phi) {//获取数据　只运行五次
switch (phi){
case 'A':
*left = 5;
*right = 1;
break;
case 'B':
*left = 1;
*right = 2;
break;
case 'C':
*left = 2;
*right = 3;
break;
case 'D':
*left = 3;
*right = 4;
break;
case 'E':
*left = 4;
*right = 5;
break;
}
}
void *eat_think(void *arg)
{
char phi = *(char *)arg;
int left,right;
get(&left,&right,phi);
//此过程不会发生死锁
for(;;){
usleep(10);
pthread_mutex_lock(&chopstick[left]);//不一定能拿到锁　可能在这里阻塞　当trylock拿到锁时
//先到则抢到一个锁　否则阻塞十毫秒左右(此人没拿上锁　意味着需要这个锁的人已拿上锁　阻塞十毫秒)

printf("Philosopher %c fetches chopstick %d\n", phi,
left);
if (pthread_mutex_trylock(&chopstick[right]) ==
EBUSY){
pthread_mutex_unlock(&chopstick[left]);
//开始随机　先运行到这里的会失去锁　相应的　当需要这个锁的哲学家到这里就会得到锁
continue;
}
printf("Philosopher %c fetches chopstick %d\n", phi,
right);
printf("Philosopher %c is eating.\n",phi);
usleep(10);
//解锁过程
pthread_mutex_unlock(&chopstick[left]);
printf("Philosopher %c release chopstick %d\n", phi,
left);
pthread_mutex_unlock(&chopstick[right]);
printf("Philosopher %c release chopstick %d\n", phi,
right);
}
}
int main(){
pthread_t A,B,C,D,E; //5个哲学家
int i;
for (i = 0; i < 5; i++)
pthread_mutex_init(&chopstick[i],NULL);

//五个哲学家共同竞争
pthread_create(&A,NULL, eat_think, (void *)"A");
pthread_create(&B,NULL, eat_think, (void *)"B");
pthread_create(&C,NULL, eat_think, (void *)"C");
pthread_create(&D,NULL, eat_think, (void *)"D");
pthread_create(&E,NULL, eat_think, (void *)"E");
//永远不会运行到这里
pthread_join(A,NULL);
pthread_join(B,NULL);
pthread_join(C,NULL);
pthread_join(D,NULL);
pthread_join(E,NULL);
return 0;
}