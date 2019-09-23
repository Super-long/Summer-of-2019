#include<iostream>
#include<algorithm>
#include<cstdio>
using namespace std;
int m,n,q;
int book[100000+10];
int segt[100000<<2];
int lazy[100000<<2];
int tmpa,tmpb,tmpc;
int ans;
#define lson (q<<1)
#define rson (q<<1|1)
#define mid ((l+r)>>1)

void build(int q,int l,int r)
{
    lazy[q]=0;
    if(l==r)
    {
        segt[q]=1;
        return;
    }
    int m=mid;
    build(lson,l,m);
    build(rson,m+1,r);
    segt[q]=segt[lson]+segt[rson];
}

void push_down(int q,int l,int r) //更新lazy标记
{
    int m=mid;
    segt[lson]=(m-l+1)*lazy[q];     //第一遍没有清空标记位　这样会导致每次更新都要下推标记　所以超时
    segt[rson]=(r-m)*lazy[q];
    lazy[lson]=lazy[q];
    lazy[rson]=lazy[q];
    lazy[q]=0;
}

void update(int q,int l,int r,int a,int b,int k) //k为要修改成的值
{
    if(l>=a && r<=b)
    {
        segt[q]=(r-l+1)*k;//segt中的是答案值
        lazy[q]=k;
        return;
    }
    int m=mid;
    if(lazy[q]) push_down(q,l,r);
    if(a<=m){
        update(lson,l,m,a,b,k);
    }
    if(b>m){
        update(rson,m+1,r,a,b,k);
    }
    segt[q]=segt[lson]+segt[rson];
}

int main()
{
    scanf("%d",&q);
    while(q--)
    {
        scanf("%d %d",&m,&n);
        build(1,1,m);
        for(int i=0;i<n;i++){
            scanf("%d %d %d",&tmpa,&tmpb,&tmpc);
            update(1,1,m,tmpa,tmpb,tmpc);
        }
        printf("Case %d: The total value of the hook is %d.\n",++ans,segt[1]);
    }
    return 0;
}