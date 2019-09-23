//裸拓扑排序　
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
#include<cstdio>
using namespace std;

int q,m,n;
vector<int>G[2000];
priority_queue<int,vector<int>,greater<int>>que;
vector<int>vec;
int in[2000];
int MAX;
int flag; //记录当前遍历的点数　用来判断环
int topo(){
    while(!que.empty()){
        int temp = que.top();
        vec.emplace_back(temp);
        ++flag;
        que.pop();
        for(auto x : G[temp]){
            if(in[x]==0) continue;
            else in[x]--;
            if(in[x]==0) que.emplace(x);
        }
    }
}

int main(){
    cin >> q;
    while(q--){
        memset(in,0,sizeof(in));
        vec.clear();
        flag = 0;
        while(!que.empty()) que.pop();
        for(int i=0;i<2000;++i) G[i].clear();
        while(cin >> m){
            MAX = max(MAX,m);
            while(cin >> n && n!=-1){
                G[n].emplace_back(m); //m指向n
                in[m]++;
                MAX = max(MAX,n);
            }
        }
        for(int i=1;i<=MAX;++i){
            if(in[i] == 0)
                que.emplace(i);
        }
        topo();
        if(flag!=MAX){
            cout << -1 << endl;
            continue;
        }
        for(auto x : vec){
            cout << x << " ";
        }
        putchar('\n');
    }
}