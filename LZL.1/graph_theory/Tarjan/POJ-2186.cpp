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

const int INF = 0x3f3f3f3f;
const int maxn = 10010;
vector<int>G[maxn];
stack<int>sta;
set<int>se;
int dfn[maxn];
int low[maxn];
int id[maxn];
int flag,ans;
int m,n,tmpa,tmpb;
int in[maxn];
int out[maxn];
int size_tmp[maxn];

void Tarjan(int start){
    se.insert(start);
    sta.push(start);
    dfn[start] = low[start] = ++flag;
    size_t size_all = G[start].size();
    for(size_t i=0;i<size_all;++i){
        int x = G[start][i];
        if(dfn[x]==-1){
            Tarjan(x);
            low[start] = min(low[start],low[x]);
        }else if(se.find(x)!=se.end()){
            low[start] = min(low[start],low[x]);
        }
    }
    if(low[start] == dfn[start]){
        while(1){
            int tmp = sta.top();
            sta.pop();
            se.erase(tmp);
            id[tmp] = ans;
            size_tmp[ans]++;
            if(start == tmp) break;
        }
        ++ans;
    }
}

void solve(){
    int tmp = 0;
    int tep = 0;
    for(int i=1;i<=n;++i){
        int x = G[i].size();
        for(int j=0;j<x;++j){
            tmp = G[i][j];
            if(id[i] != id[tmp]){
                out[id[i]]++;
                in[id[tmp]]++;
            }
        }
    }
    int temp = 0;
    for(int i=0;i<ans;++i){
        if(out[i] == 0){
            ++tep;
            temp = size_tmp[i];
        }
    }
    if(ans == 1) cout << n << endl;//整个图被缩成一个点
    else if(tep == 1) cout << temp << endl;//有可能出现一种情况就是出度为零的点为一个强连通分量
    else cout << 0 << endl;
}

int main(){
    cin >> n >> m;
    for(int i=0;i<m;i++){
        cin >> tmpa >> tmpb;
        G[tmpa].push_back(tmpb);
    }
    memset(dfn,-1,sizeof(dfn));
    for(int i=1;i<=n;++i) if(dfn[i]==-1) Tarjan(i);
    solve();
    return 0;
}
/*
4 4
4 1
1 2
2 3
3 1
3
*/