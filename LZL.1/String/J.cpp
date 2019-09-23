#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
#include<unordered_map>
#include<cstring>
using namespace std;

typedef unsigned long long ull;
const int base = 31;
char s[100000+10];
map<ull,int> mp;
int m,l;
ull h[100000+10]; //基数组
ull Hash[100000+10];
int ans;

ull BKDRhash(int l,int r)
{
    return Hash[r]-Hash[l-1]*h[r-l+1];
}

int main()
{
    h[0]=1;
    for(int i=1;i<=100000+10;i++) h[i]=h[i-1]*base;
    while(~scanf("%d %d",&m,&l))
    {
        ans=0;
        memset(Hash,0,sizeof(Hash));
        scanf("%s",s+1);
        int len=strlen(s+1);
        Hash[0]=0;
        for(int i=1;i<=len;i++)
        {
            Hash[i]=Hash[i-1]*base+s[i]-'a';//hash表建立成功
        }
        //建立超过l后会重复
        for(int i=1;i<=l&&i+m*l-1<=len;i++)
        {
            mp.clear();
            for(int j=i;j<=i+m*l-1;j+=l)
            {
                mp[BKDRhash(j,j+l-1)]++;
            }
            if(mp.size()==m) ans++;
            for(int j=i+m*l;j+l-1<=len;j+=l)
            {
                ull x=BKDRhash(j,j+l-1);
                mp[x]++;
                    x=BKDRhash(j-m*l,j-l*(m-1)-1);
                mp[x]--;
                if(mp[x]==0) mp.erase(x);
                if(mp.size()==m) ans++;
            }
        }
        printf("%d\n",ans);
    }
}