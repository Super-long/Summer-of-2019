//超级源点
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

struct node{
    int to,w;
};
vector<node> G[1005];
typedef pair<int,int> pa;
int m,n,q;
int tmpa,tmpb,tmpc;
int tmp;
int dis[1010];
int p[1010];
const int INF = 0x3f3f3f3f;

void Dijkstra(){
    priority_queue<pa,vector<pa>,greater<pa>>que;
    fill(dis,dis+1005,INF);
    dis[1003] = 0;
    que.emplace(make_pair(0,1003));
    while(!que.empty()){
        pa temp = que.top();
        que.pop();
        int v = temp.second;
        if(dis[v] < temp.first) continue;
        for(auto x : G[v]){
            if(dis[x.to] > dis[v] + x.w){
                dis[x.to] = dis[v] + x.w;
                que.emplace(make_pair(dis[x.to],x.to));
            }
        }
    }
}

int main()
{
    while(cin >> m >> n >> q){
        for(int i=0;i<1005;++i) G[i].clear();
        memset(p,0,sizeof(p));
        for(int i=0;i<m;++i){
            cin >> tmpa >> tmpb >> tmpc;
            G[tmpa].push_back({tmpb,tmpc});
            G[tmpb].push_back({tmpa,tmpc});
        }
        for(int i=0;i<n;++i){
            cin >> tmp;
            G[1003].push_back({tmp,0});
        }
        for(int i=0;i<q;++i){
            cin >> p[i];
        }
        Dijkstra();
        int MAX = INF;
        for(int i=0;i<q;++i){
            MAX = min(MAX,dis[p[i]]);
        }
        cout << MAX << endl;
    }
    return 0;
}