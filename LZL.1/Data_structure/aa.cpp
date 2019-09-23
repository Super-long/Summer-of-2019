#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;
const int maxn = 100000 + 10;
int tree[maxn];
int n;
int lowbit(int x)
{
    return x&(-x);
}
void update(int x,int val){
    while(x<=n){
        tree[x]+=val;
        x+=lowbit(x);
    }
}
int query(int x){
    int ans = 0;
    while(x > 0){
        ans += tree[x];
        x-=lowbit(x);
    }
    return ans;
}
int main()
{
    int x,y;
    while(cin >>n && n){
        memset(tree,0,sizeof(tree));
        for(int i=1;i<=n;++i){
            scanf("%d %d",&x,&y);
            update(x,1);
            update(y+1,-1);
        }
        for(int i=1;i<=n;++i){
            if(i!=1){
                printf(" ");
            }
            printf("%d",query(i));
        }
        puts("");
    }
     return 0;
}
