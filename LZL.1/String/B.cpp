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
char book[1000000+10];//母串
char res[10000+10];//子串
int flag;
int ans;

void kmp(int *next)//next第一项为-1
{
    int n=strlen(res);
    int i=0,j=-1;
    next[0]=-1;
    while(i<n)
    {
        if(j==-1 || res[i]==res[j])
        {
            i++;j++;
            next[i]=j;
        }else{
            j=next[j];
        }
    }
}

void solve(int *next)
{
    int i=0,j=0;
    int n=strlen(res);
    int m=strlen(book);
    while(i<m)
    {
        if(j==-1 || book[i]==res[j])
        {
            i++;j++;
            if(j==n)
            flag++;
        }else{
            j=next[j];
        }
    }
}

int main()
{
    int q;
    scanf("%d",&q);
    int next[10000+10];
    while(q--)
    {
        flag=0;
        scanf("%s %s",res,book);
        kmp(next);
        solve(next);
        printf("%d\n",flag);
        memset(next,0,sizeof(next));
    }
    return 0;
}