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
//数组写法　节省空间
char book[100];
int tree[500000+10][26];
int sum[500000+10];
int tot,root;

int insert(char tmp[])
{
    root=0;
    int len=strlen(tmp);
    for(int i=0;i<len;i++)
    {
        int a=tmp[i]-'a';
        if(!tree[root][a])
        tree[root][a]=++tot; //分配节点
        sum[tree[root][a]]++; //这个节点处目前的前缀
        root=tree[root][a];
    }
}

int searsh(char tmp[])
{
    int len=strlen(tmp);
    root=0;
    for(int i=0;i<len;i++)
    {
        int a=tmp[i]-'a';
        if(!tree[root][a]) return 0;
        root=tree[root][a];
    }
    return sum[root];
}

int main()
{
    while(cin.getline(book,100))
    {
        if(strlen(book)==0) break;
        insert(book);
    }
    while(cin >> book)
    {
        cout << searsh(book) << endl;
    }
    return 0;
}