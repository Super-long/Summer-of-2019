#include<iostream>
#include<map>
#include<algorithm>
#include<cstdio>
using namespace std;
int booka[4000+5];
int bookb[4000+5];
int bookc[4000+5];
int bookd[4000+5];
int book[16000000+20];
map<int,int>mp;
int m;
int tmp;
int main()
{
    scanf("%d",&m);
    for(int i=0;i<m;i++)
    scanf("%d %d %d %d",&booka[i],&bookb[i],&bookc[i],&bookd[i]);
    int t=0;
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<m;j++)
        {
            book[t++]=bookc[i]+bookd[j];
        }
    }
    sort(book,book+t);
    int sum=0;
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<m;j++)
        {
            tmp=-(booka[i]+bookb[j]);
            int a=lower_bound(book,book+t,tmp)-book;
            int b=upper_bound(book,book+t,tmp)-book;
            sum+=(b-a);
        }
    }
    printf("%d\n",sum);
}