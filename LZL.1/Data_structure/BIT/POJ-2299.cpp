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

typedef long long ll;
struct node{
    int w,index;
    bool operator<(const node &tmp)const{
        return w<tmp.w;
    }
};
ll m,n,num;
ll bit[500010];
node book[500010];

ll lowbit(ll x){
    return x&-x;
}

ll add(ll x){
    for(int i=x;i<=500010;i+=lowbit(i)){
        bit[i]+=1;
    }
}

ll sum(ll x){
    ll ans = 0;
    for(int i=x;i>0;i-=lowbit(i)){
        ans+=bit[i];
    }
    return ans;
}

int main()
{
    while(cin >> m && m){
        num = 0;
        memset(bit,0,sizeof(bit));
        for(int i=1;i<=m;++i){
            cin >> n;
            book[i].w = n;
            book[i].index = i;
        }
        sort(book+1,book+1+m);
        for(int i=m;i>=1;--i){
            num+=sum(book[i].index); //利用从后往前排除离散化留下的同一个数字离散为不同数字的情况　代码量少一点
            add(book[i].index);//再或者就是离散化费点事　同一个数字为离散为相同的值　就可以前后都ok了　总之就是好想不好写　好写不好想
        }
        cout << num << endl;
    }
    return 0;
}