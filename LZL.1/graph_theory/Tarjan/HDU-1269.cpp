#include<iostream>
#include<algorithm>
#include<string>
#include<map>
#include<set>
#include<stack>
#include<unordered_map>
#include<queue>
#include<vector>
#include<cstdlib>
#include<utility>
#include<cstring>
using namespace std;

vector<int>G[10000+10];
int m,n;
int tmpa,tmpb;
int id[10010];
int dfn[10010];
int low[10010];
int sum,flag;
stack<int>sta;
set<int>se;

int Tarjan(int start){
    low[start] = dfn[start] = ++flag;
    sta.push(start);
    se.insert(start);
    int size_all = G[start].size();
    for(int i=0;i<size_all;++i){
        int x = G[start][i];
        if(dfn[x] == -1){
            Tarjan(x);
            low[start] = min(low[start],low[x]);
        }else if(se.find(x)!=se.end()){ //在栈中　说明是同一个强连通分量　因为是一个环
            low[start] = min(low[start],low[x]);
        }
    }
    if(dfn[start] == low[start]){
        int temp = 0;
        while(1){
            temp = sta.top();
            sta.pop();
            se.erase(temp);
            id[temp] = sum;
            if(temp == start) break;
        }
        ++sum;
    }
}

int main(){
    while(cin >> m >> n){
        if(m == 0 && n == 0) break;
        for(int i=1;i<=m;++i) G[i].clear();
        memset(dfn,-1,sizeof(dfn));
        memset(low,0,sizeof(low));
        sum = 0;flag = 0;
        for(int i=0;i<n;++i){
            cin >> tmpa >> tmpb;
            G[tmpa].push_back(tmpb);
        }
        for(int i=1;i<=m;++i){
            if(dfn[i] == -1)
            Tarjan(i);
        }
        if(sum == 1) cout << "Yes\n";
        else cout << "No\n";
    }
}