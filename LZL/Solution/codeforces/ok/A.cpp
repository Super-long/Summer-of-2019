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
const int maxn=1010;
using namespace std;
map<int,int>mp;
int m,n,tmp;
int book[maxn];
int main()
{
    cin >> m >> n;
    for(int i=0;i<m;i++)
    {
        cin >> tmp;
        mp[tmp]++;
    }
    int ans=0;
    int tmep=0;
    for(int i=1;i<=n;i++)
    {
        ans+=((mp[i]/2));
    }
    cout << ((m+1)/2-ans)+ans*2;
}