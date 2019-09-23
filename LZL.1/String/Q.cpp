#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
#include<cstring>
using namespace std;

typedef unsigned long long ull;
const int base = 13331;
char s[16000000+10];
map<ull,int> mp;
int m,l;
ull h[1000000]; //基数组
ull Hash[1000000];
int ans;

ull BKDRhash(int l,int r)
{
    return Hash[r]-Hash[l-1]*h[r-l+1];
}

int main()
{
    h[0]=1;
    for(int i=1;i<=1000000+10;i++) h[i]=h[i-1]*base;
    scanf("%d %d",&l,&m);
    scanf("%s",s+1);
    int len=strlen(s+1);
    Hash[0]=0;
    for(int i=1;i<=len;i++)
    {
        Hash[i]=Hash[i-1]*base+s[i]-'a';
    }
    for(int i=1;i<=l;i++)
    {
        for(int j=i;j+l-1<=len;j+=l)
        {
            mp[BKDRhash(j,j+l-1)]++;
        }
    }
    printf("%d\n",mp.size());
    return 0;
}