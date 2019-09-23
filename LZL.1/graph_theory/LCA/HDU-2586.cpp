//lca 最近公共祖先
#include<iostream>
#include<algorithm>
#include<string>
#include<cmath>
#include<utility>
#include<vector>
#include<cstring>
using namespace std;

#define maxn 100000

int m,n,q,flag;
struct edge{
    int from,to;
    int w;
};

vector<edge>edges;
vector<int>vec[maxn];

int grand[maxn][25];//从i到距i 2的j次方远的祖先是谁
int gw[maxn][25];//从i到距i 2的j次方远的祖先之间的权值
int depth[maxn];//i的深度 开大一点 防止树链化

int addages(int x,int y,int z)
{
    edge a = {x,y,z};
    edge b = {y,x,z};
    edges.push_back(std::move(a));//根据下面存入vec[x]的标号在edges中寻找节点
    edges.push_back(std::move(b));
    vec[x].push_back(edges.size()-2);
    vec[y].push_back(edges.size()-1); //无向图
}

void dfs(int x)
{
    for(int i=1;i<=flag;i++)
    {
        grand[x][i]=grand[grand[x][i-1]][i-1];//求祖先
        gw[x][i]=gw[x][i-1]+gw[grand[x][i-1]][i-1];//求到祖先的距离
        //建图时可维护各种各样的值
    }
    int tmp=vec[x].size();
    for(int i=0;i<tmp;i++)//有向图 与之相连的要么是父亲 要么是孩子
    {
        edge e = edges[vec[x][i]];
        if(e.to!=grand[x][0]) //不等于父亲
        {
            depth[e.to]=depth[x]+1;
            grand[e.to][0]=x;
            gw[e.to][0]=e.w;
            dfs(e.to);
        }
    }
}

void init()
{
    flag = floor(log(m+0.0)/log(2.0));//n为结点数
    depth[1]=0;
    memset(grand,0,sizeof(grand));
    memset(gw,0,sizeof(gw));
    dfs(1);
}

int lca(int x,int y)
{
    if(depth[x]>depth[y]) swap(x,y);
    int ans=0;
    for(int i=flag;i>=0;i--){
        if(depth[x]<depth[y] && depth[grand[y][i]]>=depth[x]){
            ans+=gw[y][i];
            y=grand[y][i];//保证深度相同
        }
    }
    for(int i=flag;i>=0;i--){
        if(grand[x][i]!=grand[y][i]){//精髓
            ans+=gw[x][i];
            ans+=gw[y][i];
            x=grand[x][i];
            y=grand[y][i];
        }
    }
    if(x!=y){
        ans+=gw[x][0];
        ans+=gw[y][0]; //二进制博大精深
    }
    return ans;
}

int main()
{
    scanf("%d",&q);
    int tmpa,tmpb,tmpc;
    while(q--)
    {
        scanf("%d %d",&m,&n);
        for(int i=1;i<m;i++)
        {
            scanf("%d %d %d",&tmpa,&tmpb,&tmpc);
            addages(tmpa,tmpb,tmpc);
        }
        init();
        for(int i=0;i<n;i++){
            scanf("%d %d",&tmpa,&tmpb);
            printf("%d\n",lca(tmpa,tmpb));
        }
    }
    return 0;
}