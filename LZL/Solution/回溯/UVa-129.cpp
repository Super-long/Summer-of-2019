//注意格式
#include<iostream>
#include<algorithm>
#include<string>
#include<map>
#include<vector>
#include<cstring>
using namespace std;

int n,L;
char book[2000];
int tt;

int dfs(int cur)
{
    if(tt++==n)      //tt的含义 进入递归多少次 则是第几小 因为满足条件才能进入
    {
        for(int i=0;i<cur;i++)
        {
             if(i && i%4==0)
            cout << " "; 
            cout << book[i];
        }
        putchar('\n');
        cout << cur << endl;
        return 0;
    }
    for(int i=0;i<L;i++)
    {
        book[cur]='A'+i;
        int ok=0;
        for(int j=1;j*2<=cur+1;j++)
        {
            int ans=0;
            for(int k=0;k<j;k++)
            {
                if(book[cur-k]!=book[cur-k-j])
                {
                    ans=1;
                    break;
                }
            }
            if(!ans)//证明存在简单串
            {
                ok=1;
                break;
            }
        }
        if(!ok)  //存在复杂串
        {
            if(!dfs(cur+1))
            {
                return 0;
            }
        }
    }
    return 1;
}

int main()
{
    while(cin >> n >> L)
    {
        tt=0;
        memset(book,0,sizeof(book));
        if(n==0 && L==0) break;
        dfs(0);
    }
    return 0;
}