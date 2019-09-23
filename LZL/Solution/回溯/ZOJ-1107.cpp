//记忆化搜索
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<cstring>
using namespace std;
int map[110][110];
int vis[110][110];
int n,k;
int maxn;
int MAX;
int xx[4]={0,0,1,-1};
int yy[4]={1,-1,0,0};

int dfs(int x,int y)
{
    if(vis[x][y])
    return vis[x][y];
    int ans=0;
    for(int i=1;i<=k;i++)
    {
        for(int j=0;j<4;j++)
        {
            int dx=x+i*xx[j];
            int dy=y+i*yy[j];
            if(dx>=0 && dx<n && dy>=0 && dy<n && map[dx][dy]>map[x][y])    
            {
                ans=max(dfs(dx,dy),ans);
            }     
        }
    }
    return vis[x][y]=ans+map[x][y];
}

int main()
{
    while(cin >> n >> k)
    {
        if(n==-1 && k==-1)
        break;
        memset(map,0,sizeof(map));
        memset(vis,0,sizeof(vis));
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                cin >> map[i][j];
            }
        }
        cout << dfs(0,0) << endl;
    }   
    return 0;
}