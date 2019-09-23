#include<iostream>
#include<algorithm>
#include<cstring>
using namespace std;
int m,n,q;
const int maxn = 100000+10;
int tree[maxn];

int lowbit(int x)
{
    return x&-x;
}

void update(int x,int val)
{
    while(x<=n)
    {
        tree[x]+=val;
        x+=lowbit(x);
    }
}

int query(int x)
{
    int ans=0;
    while(x>0)
    {
        ans+=tree[x];
        x-=lowbit(x);
    }
    return ans;
}

int main()
{
    while(cin >> q && q)
    {
        memset(tree,0,sizeof(tree));
        for(int i=0;i<q;i++)
        {
            cin >> m >> n;
            update(m,1);
            update(n+1,-1);
        }
        for(int i=1;i<=n;++i)
        {
            if(i==1){
                printf("%d",query(i));
            }else{
                printf(" %d",query(i));
            }
        }
        putchar('\n');
    }
    return 0;
}