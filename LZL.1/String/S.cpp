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
char book[1000+10];//母串
char res[1000+10];//子串
int flag;

void kmp(int *next)
{
    next[0]=0;
    int i=1;
    int j=0;
    int n=strlen(res);
    while(i<n)
    {
        if(res[i]==res[j])
        {
            next[i]=++j;
            i++;
        }else if(j==0){
            i++;
        }else{
            j=next[j-1];
        }
    }
}

int solve(int *next)
{
    int i=0,j=0;
    int ans=0;
    int m=strlen(book);
    int n=strlen(res);
    while(i<m)
    {
        if(book[i]==res[j])
        {
            i++;
            j++;
        }else if(j==0){
            i++;
        }else{
            j=next[j-1];
        }
        if(j==n)
        {
            flag++;
            j=0;
        }
    }
}

int main()
{
    while(1)
    {
        flag=0;
        int next[1000+10];
        scanf("%s",book);
        if(book[0]=='#') break;
        scanf("%s",res);
        kmp(next);
        solve(next);
        printf("%d\n",flag);
    }
    return 0;
}