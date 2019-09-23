#include<iostream>
#include<algorithm>
#include<cstring>
#include<cstdio>
#include<stack>
using namespace std;
struct node
{
    int medium;
    int pre;
    int suf;
};
int m,n;
int book[50000+10];
struct node segt[50010<<2];
int tmpa,tmpb;
char ch[5];
#define lson (q<<1)
#define rson (q<<1|1)
#define mid ((l+r)>>1)

void push_up(int q,int len)
{
    tmpa=len-len/2;
    tmpb=len/2;
    if(tmpa==segt[lson].pre)
    {
        segt[q].pre=tmpa+segt[rson].pre;
    }else{
        segt[q].pre=segt[lson].pre;
    }
    if(tmpb==segt[rson].suf)
    {
        segt[q].suf=segt[lson].suf+tmpb;
    }else{
        segt[q].suf=segt[rson].suf;
    }
    segt[q].medium=max(max(segt[lson].medium,segt[rson].medium),segt[lson].suf+segt[rson].pre);
}

void build(int q,int l,int r)
{
    if(l==r)
    {
        segt[q].pre=segt[q].pre=segt[q].medium=1;
        return;
    }
    int m=mid;
    build(lson,l,m);
    build(rson,m+1,r);
    push_up(q,r-l+1);
}

int update(int q,int l,int r,int number,int k)
{
    if(l==number && r==number)
    {
        segt[q].medium=segt[q].pre=segt[q].suf=k;
        return 0;
    }
    int m=mid;
    if(number<=m)
    {
        update(lson,l,m,number,k);
    }else{
        update(rson,m+1,r,number,k);
    }
    push_up(q,r-l+1);
}

int query(int q,int l,int r,int number)
{
    if(l==r || segt[q].medium==(r-l+1) || segt[q].medium==0)
    {
        return segt[q].medium;
    }
    int m=mid;
    if(number<=m)
    {   
        if(number>=m-segt[lson].suf+1)
        {
            return segt[lson].suf+segt[rson].pre;
        }else{
            return query(lson,l,m,number);
        }
    }else{
        if(number<=m+segt[rson].pre-1)
        {
            return segt[rson].pre+segt[lson].suf;
        }else{
            return query(rson,m+1,r,number);
        }
    }
}

int main()
{
    while(scanf("%d %d",&m,&n))
    {
        memset(segt,0,sizeof(segt));
        stack<int>sta;
        while(n--)
        {
            scanf("%s",ch);
            getchar();
            if(ch[0]=='D')
            {
                scanf("%d",&tmpa);
                update(1,1,m,tmpa,0);
                sta.push(tmpa);
            }else if(ch[0]=='Q'){
                scanf("%d",&tmpa);
                printf("%d\n",query(1,1,m,tmpa));
            }else{
                tmpb=sta.top();
                sta.pop();
                update(1,1,m,tmpb,1);
            }
        }
    }
    return 0;
}