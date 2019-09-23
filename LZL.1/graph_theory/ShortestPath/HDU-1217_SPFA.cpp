#include<iostream>
#include<algorithm>
#include<string>
#include<map>
#include<set>
#include<unordered_set>
#include<unordered_map>
#include<queue>
#include<vector>
#include<cstdlib>
#include<utility>
#include<cstring>
using namespace std;

map<string,int>mp;
int m,n;
double w;
string tmpa,tmpb;
int flag;
struct node{
    double w;
    int to;
};
vector<node> G[36];
double dis[36];

void SPFA(){
    queue<int>que;
    unordered_set<int>se;
    memset(dis,0,sizeof(dis));
    dis[1] = 1.0;
    que.push(1);
    se.insert(1);
    while(!que.empty()){
        int temp = que.front();
        que.pop();
        se.erase(temp);
        for(auto x : G[temp]){
            if(x.w*dis[temp] > dis[x.to])
            {
                dis[x.to] = x.w*dis[temp];
                if(dis[1] > 1) return;
                if(se.find(x.to) == se.end()){
                    que.push(x.to);
                    se.insert(x.to);
                }
            }
/*             if(se.find(x.to) == se.end()){　　//这个片段是错误的　会忽略一些情况　
                if(dis[1] > 1) return; 
                if(x.w*dis[temp] > dis[x.to]){
                    dis[x.to] = x.w*dis[temp];
                    se.insert(x.to);
                    que.push(x.to);
                }
            } */
        }
    }
}

main()
{
    int cas = 0;
    while(cin >> m && m){
        flag = 0;
        mp.clear();
        for(int i=0;i<36;++i) G[i].clear();
        for(int i=0;i<m;++i){
            cin >> tmpa;
            mp[tmpa] = ++flag;
        }
        cin >> n;
        for(int i=0;i<n;++i){
            cin >> tmpa >> w >> tmpb;
            G[mp[tmpa]].push_back({w,mp[tmpb]});//没有移动构造函数　不可以使用emplace 此处要好好研究
        }
        SPFA();
        printf("Case %d: %s\n", ++cas, dis[1] > 1.0 ? "Yes" : "No");
    }
    return 0;
}