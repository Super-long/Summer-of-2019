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
const int maxn=100000+10;
using namespace std;
map<int,int>mp;
int m;
int tmpa,tmpb;
int booka[maxn];
int bookb[maxn];
int main()
{
    cin >> m;
    for(int i=0;i<m;i++)
    {
        cin >> booka[i] << bookb[i];
    }
    int flag=0;
    int sum=0;
    for(int i=0;i<m;)
    {
        tmpa=booka[i]+booka[i+2]+bookb[i+1];
        tmpb=booka[i]+bookb[i+2];
        if(tmpa>tmpb)
        {
            sum+=tmpa;
            flag=0;
        }else
    }
}