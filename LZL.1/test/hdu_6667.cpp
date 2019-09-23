#include<iostream>
#include<algorithm>
#include<string>
#include<map>
#include<set>
#include<unordered_map>
#include<list>
#include<vector>
#include<cstdlib>
#include<utility>
#include<cstring>
using namespace std;

int t;
int m,n;
long long booka[1000000+10];
long long bookb[1000000+10];
long long ans;
long long flag;
int main()
{
    static int q;
    scanf("%d",&q);
    while(q--)
    {
        flag=0;
        ans=0;
        scanf("%d",&t);
        for(int i=0;i<t;i++)
        {
            scanf("%lld %lld",booka+i,bookb+i);
            ans+=bookb[i];
        }
        for(int i=0;i<t;i++)
        {
            flag+=min(booka[i],ans-bookb[i]);
        }
        printf("%lld\n",min(flag,ans));
    }
    return 0;
}