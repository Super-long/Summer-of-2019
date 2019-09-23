#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<cstdio>
#include<cstring>
#include<unordered_map>
using namespace std;

int pre[150000+10];
int m,n;
int tmpa,tmpb;
int a,b;
int find(int i)
{
    return pre[i]==i?i:pre[i]=find(pre[i]);
}
unordered_map<int,int>mp;
int main()
{
    cin >> m >> n;
    for(int i=1;i<=m;i++)
    {
        pre[i]=i;
    }
    for(int i=0;i<n;i++)
    {
        cin >> tmpa >> tmpb;
        a=find(pre[tmpa]);
        b=find(pre[tmpb]);
        if(a!=b)
        {
            pre[a]=b;
        }
    }
    for(int i=1;i<=m;i++)
    {
        mp[pre[i]]++;
    }
    int ans=0;
    int tmp=0;
    for(const auto &x : mp)
    {
        tmp=x.second;
        ans+=(((tmp*(tmp-1))/2));
    }
    if(n==ans)
    cout << "YES\n";
    else 
    cout << "NO\n";
    return 0;
}