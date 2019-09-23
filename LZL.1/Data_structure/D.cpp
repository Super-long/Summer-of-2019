#include<iostream>
#include<algorithm>
#include<cstdio>
#define lson (q<<1)
#define rson (q<<1|1)
#define mid ((l+r)>>1)
using namespace std;
int m,n,q;
int book[200000+10];
int tmpa,tmpb;
char tmpc[10];
typedef struct 
{
    int MAX;
    int MIN;
}tree;

tree segt[(200000+10)<<2];

void build(int q,int l,int r)
{
    if(l==r)
    {
        segt[q].MAX=book[l];
        segt[q].MIN=book[l];
        return;
    }
    int m=mid;
    build(lson,l,m);
    build(rson,m+1,r);
    segt[q].MAX=max(segt[lson].MAX,segt[rson].MAX);
    segt[q].MIN=min(segt[lson].MIN,segt[rson].MIN);
}

int query_max(int q,int l,int r,int a,int b)
{
    if(l>=a && r<=b)
    {
        return segt[q].MAX;
    }
    int MAX=0;
    int m=mid;
    if(a<=m){
        MAX=max(MAX,query_max(lson,l,m,a,b));
    }
    if(b>m){
        MAX=max(MAX,query_max(rson,m+1,r,a,b));
    }
    return MAX;
}

int query_min(int q,int l,int r,int a,int b)
{
    if(l>=a && r<=b)
    {
        return segt[q].MIN;
    }
    int MIN=0x3f3f3f3f;
    int m=mid;
    if(a<=m){
        MIN=min(MIN,query_min(lson,l,m,a,b));
    }
    if(b>m){
        MIN=min(MIN,query_min(rson,m+1,r,a,b));
    }
    return MIN;
}

int main()
{
    scanf("%d %d",&tmpa,&tmpb);
    for(int i=1;i<=tmpa;i++)
    {
        scanf("%d",&book[i]);
    }
    build(1,1,tmpa);
    for(int i=0;i<tmpb;i++)
    {
        scanf("%d %d",&m,&n);
        printf("%d\n",query_max(1,1,tmpa,m,n)-query_min(1,1,tmpa,m,n));
    }
    return 0;
}