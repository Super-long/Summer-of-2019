//四色定理
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<cstring>
using namespace std;
int m,n;
char book[40];
vector<int>vec[30];
int ans=0;
int map[30][30];
int dd[40];

int judge(int node)
{
    if(node==m) return 0;
    for(int i=1;i<=4;i++)
    {
        int flag=0;
        for(int j=0;j<m;j++)
        {
            if(map[node][j] && dd[j]==i)//存在且染色重复
            {
                flag=1;
                break;
            }
        }
        if(!flag)//可以染此种颜色
        {
            dd[node]=i;   //给node+‘A’号节点染色
            judge(node+1);
            break;
        }
    }
    return 0;
}

int main()
{
    while(cin >> m)
    {
        if(!m)break;
        memset(book,0,sizeof(book));
        memset(map,0,sizeof(map));
        for(int i=0;i<m;i++)
        {
            cin >> book;
            for(int j=0;j<strlen(book)-2;j++)
            {
                map[i][book[j+2]-'A']=1;
                map[book[j+2]-'A'][i]=1;
            }
        }
        judge(0);
        sort(dd,dd+m);
        int temp=dd[m-1];
        if(temp==1)
        cout << temp << " channel needed.\n";
        else 
        cout << temp << " channels needed.\n";
    }
}