#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
#include<unordered_map>
#include<cstring>
using namespace std;
char book[100];
int tree[300000+10][26];
int sum[300000+10];
int tot,root;
int m,n,tt;

int insert(char tmp[])
{
    root=0;
    int len=strlen(tmp);
    for(int i=0;i<len;i++)
    {
        int a=tmp[i]-'A';
        if(!tree[root][a])
        tree[root][a]=++tot;
        sum[tree[root][a]]++;
        root=tree[root][a];
    }
}

int searsh(char tmp[])
{
    int len=strlen(tmp);
    root=0;
    for(int i=0;i<len;i++)
    {
        tt++;
        int a=tmp[i]-'A';
        if(sum[tree[root][a]]!=n)
        {
            return 0;
        }
        root=tree[root][a];
    }
    return 1;
}
int main()
{
    scanf("%d",&m);
    while(m--)
    {
        tt=0;
        scanf("%d",&n);
        for(int i=0;i<n;i++)
        {
            scanf("%s",book);
            insert(book);
        }
        searsh(book);
        printf("%d\n",tt*n);
    }
}