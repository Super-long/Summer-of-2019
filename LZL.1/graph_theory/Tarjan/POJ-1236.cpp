//并查集只能是双向关系　单向关系缩点，寻找入度为零　更好
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

int q,m,n,tmp;
vector<int>G[1010];
set<int>se;//记录栈中元素
stack<int>sta;
int id[1010];//缩点
int size_all[1010];
int dfn[1010];
int low[1010];
int in[1010];//入度
int out[1010];//出度
int flag,num,tmpa;
int in_tmp,out_tmp;

int Tarjan(int start)
{
    se.insert(start);
    sta.push(start);
    dfn[start] = low[start] = ++flag;
    int size_for_tmp = G[start].size();
    for(int i=0;i<size_for_tmp;++i){
        int x = G[start][i];
        if(dfn[x] == -1){
            Tarjan(x);
            low[start] = min(low[start],low[x]);
        }else if(se.find(x)!=se.end()){
            low[start] = min(low[start],low[x]);
        }
    }
    if(dfn[start] == low[start]){
        int temp = 0;
        while(1)
        {
            temp = sta.top();
            sta.pop();
            se.erase(temp);
            size_all[num]++;
            id[temp] = num;
            if(temp == start) break;
        }
        ++num;
    }
}

int main()
{
    cin >> q;
    for(int i=1;i<=q;i++){
        while(cin >> tmpa && tmpa!=0){
            G[i].push_back(tmpa);
        }
    }
    memset(dfn,-1,sizeof(dfn));
    for(int i=1;i<=q;++i) 
    if(dfn[i]==-1)  Tarjan(i);
    for(int i=1;i<=q;i++){
        int size_for_tmp = G[i].size();
        for(int j=0;j<size_for_tmp;++j){
            int x = G[i][j];
            if(id[x] != id[i]){ //不在一个强连通分量中
                out[id[i]]++;
                in[id[x]]++;
            }
        }
    }
    for(int i=0;i<num;i++){
        if(in[i] == 0) ++in_tmp;
        if(out[i] == 0) ++out_tmp;
    }
    out_tmp = max(in_tmp,out_tmp) ;
    if (num == 1){ //特殊情况　图是一个强连通分量　答案显然就是1和0
        in_tmp = 1;
        out_tmp = 0;
    }
    cout << in_tmp << endl << out_tmp << endl;
    return 0;
}