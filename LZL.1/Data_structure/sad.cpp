#include<iostream>
#include<algorithm>
#include<cstring>
#include<cstdio>
#include<stack>
#define lson (q<<1)
#define rson (q<<1|1)
#define mid ((l+r)>>1)
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

void push_up(int q,int len)
{
    int tmpa=(len-len/2);
    int tmpb=len/2;
    if(segt[lson].pre==tmpa)
    {
        segt[q].pre=tmpa+segt[rson].pre;
    }else
    {
        segt[q].pre=segt[lson].pre;
    }
    if(segt[rson].suf==tmpb)
    {
        segt[q].suf=tmpb+segt[lson].suf;
    }else
    {
        segt[q].suf=segt[rson].suf;
    }
    segt[q].medium=max(max(segt[lson].medium,segt[rson].medium),segt[lson].suf+segt[rson].pre);
}

void build_tree(int q,int l,int r)
{
    if(l==r)
    {
        segt[q].pre=segt[q].medium=segt[q].suf=1;
        return;
    }
    int m=mid;
    build_tree(lson,l,m);
    build_tree(rson,m+1,r);
    push_up(q,r-l+1);
}

void update(int q,int l,int r,int number,int k)
{
    if(l==number && r==number)
    {
        segt[q].pre=segt[q].medium=segt[q].suf=k;
        return;
    }
    int m=mid;
    if(number<=m) update(lson,l,m,number,k);
    else update(rson,m+1,r,number,k);
    push_up(q,r-l+1);
    return;
}

int query(int q,int l,int r,int k)
{
    if(l==r || segt[q].medium==(r-l+1) || segt[q].medium==0)
    {
        return segt[q].medium;
    }
    int m=mid;
    if(k<=mid)
    {
        if(k>=m-segt[lson].suf+1)
        {
            return segt[lson].suf+segt[rson].pre;
        }else
        {
            return query(lson,l,m,k);
        }
    }
    else
    {
        if(k<=m+1+segt[rson].pre-1)
        {
            return segt[lson].suf+segt[rson].pre;
        }else
        {
            return query(rson,m+1,r,k);
        }
    }
}
int main()
{
    while(~scanf("%d %d",&m,&n))
    {
        stack<int> st;
        build_tree(1,1,m);
        while(n--)
        {
            scanf("%s",ch);
            getchar();
            if(ch[0]=='R')
            {
                if(!st.empty())
                update(1,1,m,st.top(),1);
                st.pop();
                continue;
            }
            if(ch[0]=='D')
            {
                scanf("%d",&tmpa);
                st.push(tmpa);
                update(1,1,m,tmpa,0);
            }else
            {
                scanf("%d",&tmpa);
                printf("%d\n",query(1,1,m,tmpa));
            }
        }
    }
    return 0;
}