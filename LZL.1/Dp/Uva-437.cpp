#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
#include<unordered_map>
using namespace std;
int m;
int x,y,z;
int ans,flag;
typedef struct 
{
    int l,w,h;
}node_t;
node_t book[3000+10];
int d[3000+10];

int graph(int a,int b,int c)
{
    book[ans].l=max(a,b);
    book[ans].w=min(a,b);
    book[ans++].h=c;
}

vector<int> vec[3000+10];

int dp(int j)
{
    if(d[j]>0) return d[j];
    d[j]=book[j].h;
    for(auto x : vec[j])
    {
        d[j] = max(d[j],dp(x)+book[j].h);
    }
    return d[j];
}

int create()
{
    for(int i=1;i<ans;i++) vec[i].clear();
    for(int i=1;i<ans;i++)
    {
        for(int j=1;j<ans;j++)
        {
            if(book[i].l>book[j].l && book[i].w>book[j].w)
            vec[i].push_back(j);
        }
    }
}

int main()
{
    int number=0;
    while(cin >> m && m)
    {
        memset(d,0,sizeof(d));
        flag=0;
        ans=1;
        for(int i=0;i<m;i++)
        {
            cin >> x >> y >> z;
            graph(x,y,z);
            graph(z,x,y);
            graph(y,z,x);
        }
        create();
        for(int i=1;i<ans;i++)
        {
            flag=max(flag,dp(i));
        }
        printf("Case %d: maximum height = %d\n",++number,flag);
    }
    return 0;
}