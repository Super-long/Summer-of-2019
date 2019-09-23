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
int segt[200010<<2];

void build(int q,int l,int r)
{
    if(l==r){
        segt[q]=book[l];
        return;
    }
    int m=mid;
    build(lson,l,m);
    build(rson,m+1,r);
    segt[q]=max(segt[lson],segt[rson]);
}

void update(int q,int l,int r,int number,int k)
{
    if(l==r)
    {
        segt[q]=k;
        return;
    }
    int m=mid;
    if(number<=m) update(lson,l,m,number,k);
    if(number>m) update(rson,m+1,r,number,k);
    segt[q]=max(segt[lson],segt[rson]);
}

int query(int q,int l,int r,int a,int b)
{
    if(l>=a && r<=b)
    {
        return segt[q];
    }
    int m=mid;
    int MAX=0;
    if(a<=m) MAX=max(query(lson,l,m,a,b),MAX);
    if(b>m)  MAX=max(query(rson,m+1,r,a,b),MAX);
    return MAX;
}

int main()
{
    while(~scanf("%d %d",&m,&n))
    {
        for(int i=1;i<=m;i++)
        {
            scanf("%d",&book[i]);
        }
        build(1,1,m);
        while(n--)
        {
            scanf("%s %d %d",tmpc,&tmpa,&tmpb);
            if(tmpc[0]=='Q')
            printf("%d\n",query(1,1,m,tmpa,tmpb));
            else 
            update(1,1,m,tmpa,tmpb);
        }
    }
    return 0;
}