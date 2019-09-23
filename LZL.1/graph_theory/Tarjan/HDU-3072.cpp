//Tarjan + 最小树型图(朱刘算法)
//应该是Tarjan+贪心 或者最小树型图

//这道题对于强连通分量中的点不要求值　所以可以Tarjan+贪心　有些题必须用最小树型图　HDU2121
#include<iostream>
#include<algorithm>
#include<string>
#include<map>
#include<set>
#include<map>
#include<set>
#include<queue>
#include<stack>
#include<vector>
#include<cstdlib>
#include<utility>
#include<cstring>
using namespace std;

int m,n;
int tmpa,tmpb,tmpc;
int edge[300][300];
vector<int>G[51000];
stack<int>sta;
set<int>se;
const int INF = 0x3f3f3f3f;
int xx[51000*2],yy[51000*2],zz[51000*2];
int dfn[51000];
int low[51000];
int id[51000];
int size_all[51000];
int sum,flag,ans;

int Tarjan(int start){
    se.insert(start);
    sta.push(start);
    dfn[start] = low[start] = ++flag;
    int size_All = G[start].size();
    for(int i=0;i<size_All;++i){
        int x = G[start][i];
        if(dfn[x] == -1){
            Tarjan(x);
            low[start] = min(low[start],low[x]);
        }else if(se.find(x) != se.end()){
            low[start] = min(low[start],low[x]);
        }
    }
    if(dfn[start] == low[start]){
        while(1){
            int x = sta.top();
            sta.pop();
            se.erase(x);
            id[x] = sum;
            size_all[sum]++;
            if(start == x) break;
        }
        ++sum;
    }
}

void solve(){
    bool vis[51000];

    for(int i=0;i<n;i++){
        int a = xx[i],b = yy[i];
        if(id[a] != id[b]){
            edge[id[a]][id[b]] = min(edge[id[a]][id[b]],zz[i]);
        }
    }
    memset(vis,false,sizeof(vis));

    for(int i=0;i<sum;++i){
        int temp = INF,tmp=0; //注意赋初值

        for(int j=0;j<sum;++j){
            for(int k=0;k<sum;++k){
                if(!vis[k] && edge[j][k]<temp){
                    temp = edge[j][k]; //每个最外层循环可以找到一个最小值　下一次寻找次小值
                    tmp = k;
                }
            }
        }
        //原因是因为最后总有一点入度为零
        if(temp != INF)//注意判断INF 
        ans+=temp;
        vis[tmp] = true;
    }
    printf("%d\n",ans);
}

int main()
{
    while(~scanf("%d %d",&m,&n)){
        for(int i=0;i<m;++i) G[i].clear();
        for(int i=0;i<n;++i){
            scanf("%d %d %d",&xx[i],&yy[i],&zz[i]);
            G[xx[i]].push_back(yy[i]);
        }
        memset(dfn,-1,sizeof(dfn));
        memset(low,0,sizeof(low));
        memset(id,0,sizeof(id));
        while(!sta.empty()) sta.pop();
        sum = 0; flag = -1,ans = 0;
        se.clear();
        for(int i=0;i<m;++i){
            if(dfn[i] == -1) Tarjan(i);
        }
        memset(edge,INF,sizeof(edge));
        solve();
    }
    return 0;
}