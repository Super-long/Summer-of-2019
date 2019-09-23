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
ull Hash[1000000+10];
ull base[1000000+10];
const int prime=13331;
char str[1000000+10];
int ans;

ull BKDRhash(int l,int r)
{
    return Hash[r]-Hash[l-1]*base[r-l+1];
}
map<ull,int>mp;
int main()
{
    base[0]=1;
    for(int i=1;i<1000000+10;i++){
        base[i]=base[i-1]*prime;
    }
    while(scanf("%s",str+1)){
        if(str[1]=='.') break;
        Hash[0]=0;
        int x=strlen(str+1);
        for(int i=1;i<=x;i++)
        Hash[i]=Hash[i-1]*prime+str[i]-'a';
        for(int i=1;i<=x;i++)
        {
            if(x%i) continue;
            mp.clear();
            for(int j=1;j+i-1<=x;j+=i)
            {
                mp[BKDRhash(j,j+i-1)]++;
            }
            if(mp.size()==1)
            {
                printf("%d\n",x/i);
                break;
            }
        }
    }
}