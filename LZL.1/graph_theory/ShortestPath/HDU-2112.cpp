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

string start_,end_;
string from,to;
unordered_map<string,int>mp;
const int INF = 0x3f3f3f3f;
int w,q,sum;
int flag;
struct edge{
    int to,cost;
};
int dis[10000+10];
vector<edge>G[10000+10];
typedef pair<int,int> pa;

int Dijkstra(int s){
    priority_queue<pa,vector<pa>,greater<pa>>que;
    fill(dis,dis+10000+5,INF);
    dis[s]=0; //到自身的情况
    que.emplace(make_pair(0,s));
    while(!que.empty())
    {
        pa p = que.top();
        que.pop();
        int v = p.second;
        if(dis[v]<p.first) continue;
        size_t len = G[v].size();
        for(size_t i=0;i<len;++i){
            edge temp = G[v][i];
            if(dis[temp.to]>temp.cost+dis[v]){
                dis[temp.to]=temp.cost+dis[v];
                que.emplace(make_pair(dis[temp.to],temp.to));
            }
        }
    }
}

int judge(string &tmpa){
    if(mp.find(tmpa)==mp.end())
    mp[tmpa]=++flag;
}

int main()
{
    while(cin >> q){
        if(q==-1) break;
        mp.clear();
        for(int i=0;i<10005;++i) G[i].clear();
        flag = 0;
        cin >> start_ >> end_;
        judge(start_); judge(end_);  //可能出现起点或终点在下方未出现
        for(int i=0;i<q;++i)
        {
            cin >> from >> to >> sum;
            judge(from); judge(to);
            edge tmp;
            tmp.to = mp[to];
            tmp.cost = sum;
            G[mp[from]].emplace_back(tmp);
            tmp.to = mp[from];
            G[mp[to]].emplace_back(tmp);
        }
        Dijkstra(mp[start_]);
        if(dis[mp[end_]]!=INF){
            cout << dis[mp[end_]] << endl;
        }else{
            cout << "-1\n";
        }
    }
    return 0;
}