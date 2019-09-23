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
long long sum=0;
int m,n;
string book[maxn];
int main()
{
    cin >> m;
    for(int i=1;i<=m;i++)
    {
        cin >> book[i];
    }
    for(int i=1;i<=m;i++)
    {
        for(int j=1;j<=m;j++)
        {
            int ans=book[i].size()+book[j].size();
            string str('0',ans);
            int tmpa=book[i].size();
            int tmpb=book[j].size();
            int flag=0;
            int tt=1,gg=1;
            for(int k=ans-1;k>=0;k++)
            {
                if(!flag && tt)
                {
                    str[k]=book[j][tmpb--];
                    if(tmpb>=0 && gg) flag=!flag;
                    else
                    {
                        tt=0;
                        flag=!flag;
                    }
                }else if(flag && gg)
                {
                    str[k]=book[i][tmpa--];
                    if(tmpa>=0 && tt) flag=!flag;
                    else
                    {
                        gg=0;
                        flag=!flag;
                    }
                }
            }
            cout << str << endl;
        }
    }
}