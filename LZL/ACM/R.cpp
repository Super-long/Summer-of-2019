/* #include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
using namespace std;
int m,n;
int tmpa,tmpb;
int pre[50000+10];
int dd;
int find(int i)
{
    return pre[i]==i?i:(pre[i]=find(pre[i]));
}
void init(int x)
{
    for(int i=1;i<=x;i++) pre[i]=i;
}
int main()
{
    int ans(0);
    while(scanf("%d %d",&m,&n))
    {
        init(m);
        dd=m;
        if(!m && !n) return 0;
        for(int i=0;i<n;i++)
        {
            scanf("%d %d",&tmpa,&tmpb);
            int x=find(pre[tmpa]);
            int y=find(pre[tmpb]);
            if(x!=y)
            {
                pre[y]=x;
                dd--;
            }
        }
        printf("Case %d: %d\n",++ans,dd);
    }
} */

#include<iostream>   //不能用ｍａｐ来判断并查集种类数
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
using namespace std;
map<int,int>mp;
int m,n;
int tmpa,tmpb;
int pre[50000+10];
int find(int i)
{
    return pre[i]==i?i:(pre[i]=find(pre[i]));
}
void init(int x)
{
    for(int i=1;i<=x;i++) pre[i]=i;
}
int main()
{
    int ans(0);
    while(scanf("%d %d",&m,&n))
    {
        init(m);
        ++ans;
        if(!m && !n) return 0;
        for(int i=0;i<n;i++)
        {
            scanf("%d %d",&tmpa,&tmpb);
            int x=find(pre[tmpa]);
            int y=find(pre[tmpb]);
            if(x!=y)
            pre[y]=x;
        }
        for(int i=1;i<=m;i++)
        {
            cout << pre[i] << endl;
            mp[pre[i]]++;
        }
        printf("Case %d: %d\n",ans,mp.size());
        mp.clear();
    }
}