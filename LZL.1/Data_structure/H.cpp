#include<iostream>
#include<algorithm>
#include<cstdio>
using namespace std;
int m,n,q;
int book[50000+10];
int segt[50000<<2];
int tmpa,tmpb;
char tmpc[10];
#define lson (q<<1)
#define rson (q<<1|1)
#define mid ((l+r)>>1)


void push_up(int q)
{
    segt[q]=segt[lson]+segt[rson];
}

void build(int q ,int l,int r)
{
    if(l==r)
    {
        segt[q]=book[l];
        return;
    }
    int m=mid;
    build(lson,l,m);
    build(rson,m+1,r);
    push_up(q);
}

void update_add(int q,int l,int r,int number,int k)
{
    if(l==r)
    {
        segt[q]+=k;
        return;
    }
    int m=mid;
    if(number<=m) update_add(lson,l,m,number,k);
    else update_add(rson,m+1,r,number,k);
    push_up(q);
}

void update_sub(int q,int l,int r,int number,int k)
{
    if(l==r)
    {
        segt[q]-=k;
        return;
    }
    int m=mid;
    if(number<=m) update_sub(lson,l,m,number,k);
    else update_sub(rson,m+1,r,number,k);
    push_up(q);
}

int query(int q,int l,int r,int a,int b)
{
    int sum=0;
    if(l>=a && r<=b)
    {
        sum+=segt[q];
        return sum;
    }
    int m=mid;
    if(a<=m) sum+=query(lson,l,m,a,b);
    if(b>m) sum+=query(rson,m+1,r,a,b);
    return sum;
}

int main()
{
    scanf("%d",&q);
    int ans=0;
    while(q--)
    {
        scanf("%d",&m);
        for(int i=1;i<=m;i++){
            scanf("%d",&book[i]);
        }
        build(1,1,m);
        printf("Case %d:\n",++ans);
        while(1){
            scanf("%s",tmpc);
            if(tmpc[0]=='E') break;
            scanf("%d %d",&tmpa,&tmpb);
            if(tmpc[0]=='A')
            {
                update_add(1,1,m,tmpa,tmpb);
            }else if(tmpc[0]=='S'){
                update_sub(1,1,m,tmpa,tmpb);
            }else{
                printf("%d\n",query(1,1,m,tmpa,tmpb));
            }
        }
    }
    return 0;
}