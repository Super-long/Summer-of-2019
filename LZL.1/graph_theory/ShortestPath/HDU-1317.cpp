//folyd + bellman 
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

int f[110][110],p[110];
int q,m,n,flag,sum;
int tmpa,tmpb,tmpc;
struct node{
    int from,to;
};
node e[10000+10];
int dis[110];
static const int INF = -0x3f3f3f3f;

int folyd(int n){
    for(int i=1;i<=n;++i){
        for(int j=1;j<=n;++j){
            for(int k=1;k<=n;++k){
                if(f[j][i]&&f[i][k]){ //可达性矩阵
                    f[j][k]=1;
                }
            }
        }
    }
    if(f[1][n]==0) return 1; //起点无法到达终点
    return 0;
}

int bellman(){
    fill(dis,dis+105,INF);
    dis[1] = 100;
    for(int i=1;i<=q;++i){
        for(int j=1;j<flag;++j){
            if(dis[e[j].from]+p[e[j].to]>dis[e[j].to] && dis[e[j].from]+p[e[j].to]>0){
                dis[e[j].to] = dis[e[j].from]+p[e[j].to];
            }
        }
    }
    for(int i=1;i<flag;++i){//存在正环且与终点相连 第三个条件　环中任意一点到终点可达
        if(dis[e[i].from]+p[e[i].to]>dis[e[i].to] && dis[e[i].from]+p[e[i].to]>0 && f[e[i].from][q]) return 1;
    } //很容易求出环　O(N*M)
    if(dis[q] > 0) return 1;//最大权
    return 0;
}

int main()
{
    while(cin >> q && q!=-1){
        flag = 1;
        memset(f,0,sizeof(f));
        memset(p,0,sizeof(p));
        for(int i=1;i<=q;++i){
            cin >> p[i] >> n;
            while(n--){
                cin >> m;
                f[i][m]=1;
                e[flag].from = i;
                e[flag].to  = m;
                flag++;
            }
        }
        if(folyd(q)) cout << "hopeless\n";
        else if(bellman()) cout << "winnable\n";
        else cout << "hopeless\n";
    }
    return 0;
}