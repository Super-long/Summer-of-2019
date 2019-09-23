//变种迪杰斯特拉
#include<iostream>
#include<algorithm>
#include<string>
#include<map>
#include<set>
#include<queue>
#include<vector>
#include<cstdio>
#include<cstdlib>
#include<utility>
#include<cstring>
using namespace std;

int flag;
int q,m,n;
int tmpa,tmpb,tmpc;
struct node{
    int to,w;
    bool operator<(node &tmp){
        return w<tmp.w;
    }
};
vector<node>G[1005];
int dis[1005];
const int INF = 0x3f3f3f3f;
typedef pair<int,int> pa;

void Dijkstra(){
    priority_queue<pa> que;
    memset(dis,0,sizeof(dis));
    dis[1] = INF;
    que.push(make_pair(INF,1));
    while(!que.empty()){
        pa p = que.top();
        que.pop();
        int index = p.second;
        if(dis[index] > p.first) continue;
        int sum = G[index].size();
        for(int i=0;i<sum;++i){
            node x = G[index][i];
            if(dis[x.to] < min(x.w,dis[index])){
                dis[x.to] = min(x.w,dis[index]);
                que.push(make_pair(dis[x.to],x.to));
            }
        }
    }
}

int main()
{
    scanf("%d",&q);
    ++q;
    while(--q){
        scanf("%d %d",&m,&n);
        for(int i=0;i<n;++i){
            scanf("%d %d %d",&tmpa,&tmpb,&tmpc);
            G[tmpa].push_back({tmpb,tmpc});//为什么emplace不能直接赋予
            G[tmpb].push_back({tmpa,tmpc});
        }
        Dijkstra();
        for(int i=1;i<=m;++i) G[i].clear();
        cout << "Scenario #" << ++flag << ":\n" << dis[m] << endl;
        cout << endl;
    }
    return 0;
}