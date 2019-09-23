#include<iostream>
#include<algorithm>
#include<string>
#include<map>
#include<set>
#include<unordered_map>
#include<queue>
#include<vector>
#include<cstdlib>
#include<utility>
#include<cstring>
using namespace std;

int m,n,q;
int ans;
int tmp;
const int INF = 0x3f3f3f3f;
const int maxn = 205;
int vis[maxn];
int dis[maxn];
vector<int> G[maxn];

void Dijkstra(){
    priority_queue<int,vector<int>,greater<int>>que;
    fill(dis,dis+maxn,INF);
    dis[m] = 0;
    que.emplace(m);
    while(!que.empty()){
        int temp = que.top();
        que.pop();
        for(auto x : G[temp]){
            if(dis[x] > dis[temp]+1){
                dis[x] = dis[temp] + 1;
                que.emplace(x);
            }
        }
    }
}

int main()
{
    while(cin >> q && q){
        cin >> m >> n;
        memset(vis,0,sizeof(vis));
        for(int i=0;i<maxn;++i) G[i].clear();
        for(int i=1;i<=q;++i){
            cin >> vis[i];
        }
        for(int i=1;i<=q;++i){
            if(i-vis[i]>=1) G[i].emplace_back(i-vis[i]);
            if(i+vis[i]<=q) G[i].emplace_back(i+vis[i]);
        }
        Dijkstra();
        if(dis[n] != INF)
        cout << dis[n] << endl;
        else 
        cout << -1 << endl;
    }
    return 0;
}