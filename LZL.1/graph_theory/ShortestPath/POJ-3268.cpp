#include<iostream>
#include<algorithm>
#include<string>
#include<map>
#include<set>
#include<map>
#include<queue>
#include<vector>
#include<cstdlib>
#include<utility>
#include<cstring>
using namespace std;

struct node{
    int to,w;
};
vector<node> G[1010];
vector<node>GG[1010];
typedef pair<int,int> pa;
int dis[1010];
int dd[1010];
int m,n,q;
int tmpa,tmpb,tmpc;
static const int INF = 0x3f3f3f3f;

void Dijkstra(const vector<node> vec[],const int x,int (&dis)[1010]){
    priority_queue<pa,vector<pa>,greater<pa> > que;
    fill(dis,dis+1005,INF);
    que.push(make_pair(0,x));
    dis[x] = 0;
    while(!que.empty()){
        pa temp= que.top();
        que.pop();
        int v = temp.second;
        if(dis[v] < temp.first) continue;
        int sum = vec[v].size();
        for(int i=0;i<sum;++i){
            node x = vec[v][i];
            if(dis[v] + x.w < dis[x.to]){
                dis[x.to] = dis[v] + x.w;
                que.push(make_pair(dis[x.to],x.to));
            }
        }
    }
}

int main()
{
    cin >> m >> n >> q;
    for(int i=0;i<n;++i){
        cin >> tmpa >> tmpb >> tmpc;
        G[tmpa].push_back({tmpb,tmpc});
        GG[tmpb].push_back({tmpa,tmpc});
    }
    Dijkstra(G,q,dis);
    Dijkstra(GG,q,dd);
    for(int i=1;i<=m;++i){
        dis[i]+=dd[i];
    }
    sort(dis+1,dis+m+1);
    cout << dis[m] << endl;
    return 0;
}