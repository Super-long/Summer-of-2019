//ｎ堆硬币　面值不同　无限　填满一个数字　最大数与最小数
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
#include<unordered_map>
using namespace std;

int d[200];
int v[200];
int dp(int s)
{
    int  &ans=d[s];
    if(ans>=0) return ans;
    int ans=-(1<<30);  //区分无法达到０与真实达到０的区别
    for(int i=0;i<20;i++)
    {
        if(s>=v[i])
        {
            ans=max(ans,dp(ans-v[i])+1);
        }
    }
    return ans;
}

int traverse(int s)//打印路径
{
    for(int i=0;i<20;i++)
    {
        if(s>=v[i] && d[s]==d[s-v[i]]+1)
        {
            cout << v[i] << endl;
            traverse(s-v[i]);
            break;
        }
    }
}
int main()
{
    memset(d,-1,sizeof(d));
    d[0]=0;    //搜索终止条件
}