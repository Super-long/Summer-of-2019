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

map<string,int>mp;
int q,p;
double ans;
double vis[40][40];
string tmpa,tmpb;
unsigned int flag;
unsigned int eg;

int main()
{
    while(cin >> q && q!=0){
        flag=0;
        memset(vis,0,sizeof(vis));
        mp.clear();
        for(int i=0;i<q;++i){
            cin >> tmpa;
            mp[tmpa]=++flag;
            vis[flag][flag]=1;
        }
        cin >> p;
        for(int i=0;i<p;++i){
            cin >> tmpa >> ans >> tmpb;
            vis[mp[tmpb]][mp[tmpa]] = ans;
        }
        for(int i=1;i<=q;++i){
            for(int j=1;j<=q;++j){
                for(int k=1;k<=q;++k){
                    vis[j][k]=max(vis[j][k],vis[j][i]*vis[i][k]);
                }
            }
        }
        int key=0;
        for(int i=1;i<=q;++i){
            if(vis[i][i]>1){
                key=1;
                break;
            }
        }
        printf("Case %d: %s\n",++eg,key?"Yes":"No");
    }
    return 0;
}