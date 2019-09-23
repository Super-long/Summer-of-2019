#include<iostream>
#include<algorithm>
#include<string>
#include<map>
#include<set>
#include<unordered_map>
#include<list>
#include<vector>
#include<cstdlib>
#include<utility>
#include<cstring>
using namespace std;

int mp[110][110];
int m,n,q;
int tmpa,tmpb,tmpc;
int flag;

void init()
{
    for(int i=1;i<=105;i++){
        for(int j=1;j<=105;j++){
            if(i==j) mp[i][j]=0;
            else mp[i][j]=0x3f3f3f3f;
        }
    }
}

int main()
{
    while(scanf("%d %d %d",&m,&n,&q))
    {
        if(m==0 && n==0 && q==0) break;
        init();
        for(int i=0;i<n;i++){
            scanf("%d %d %d",&tmpa,&tmpb,&tmpc);
            mp[tmpa][tmpb]=tmpc;
            mp[tmpb][tmpa]=tmpc;
        }
        for(int i=1;i<=m;i++){
            for(int j=1;j<=m;j++){
                for(int k=1;k<=m;k++){
                    mp[j][k]=min(mp[j][k],max(mp[j][i],mp[i][k]));
                }
            }
        }
        if(flag++)
        putchar('\n');
        printf("Case #%d\n",flag);
        for(int i=0;i<q;i++){
            scanf("%d %d",&tmpa,&tmpb);
            if(mp[tmpa][tmpb]!=0x3f3f3f3f)
            printf("%d\n",mp[tmpa][tmpb]);
            else 
            printf("no path\n");
        }
    }
    return 0;
}