#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
#include<unordered_map>
#include<cstring>
#include<sstream>
using namespace std;

int tree[200000+10][26];
int num[200000+10];
int tot,root,ans;
string str,temp;

int join(string &str)
{
    root=1;
    int x=str.size();
    for(int i=0;i<x;i++)
    {
        int t=str[i]-'a';
        if(tree[root][t]==0)
        tree[root][t]=++tot;
        root=tree[root][t];
    }
    if(num[root]==0)
    {
        ans+=1;
        num[root]=1;
    }
    return 0;
}

int main()
{
    while(getline(cin,str) && str[0]!='#')
    {
        ans=0;
        tot=0;
        memset(tree,0,sizeof(tree));
        memset(num,0,sizeof(num));
        stringstream stream(str);
        while(stream >> temp)
        {
            join(temp);
        }
        cout << ans << endl;
    }
    return 0;
}

//没有过　不知道为什么

/*#include<iostream>  
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
#include<unordered_map>
#include<cstring>
#include<sstream>
using namespace std;

int tree[200000+10][26];
int num[200000+10];
int tot,root;
string str,temp;

int join(string str)
{
    root=0;
    int x=str.size();
    for(int i=0;i<x;i++)
    {
        int t=str[i]-'a';
        if(tree[root][t]==0)
        tree[root][t]=++tot;
        num[tree[root][t]]++;
        if(i==x-1 && num[tree[root][t]]==1)
        return 1;
        root=tree[root][t];
    }
    return 0;
}

int main()
{
    int ans=0;
    while(getline(cin,str) && str[0]!='#')
    {
        ans=0;
        tot=0;
        memset(tree,0,sizeof(tree));
        memset(num,0,sizeof(num));
        stringstream stream(str);
        while(stream >> temp)
        {
            ans+=join(temp);
        }
        cout << ans << endl;
    }
    return 0;
}*/