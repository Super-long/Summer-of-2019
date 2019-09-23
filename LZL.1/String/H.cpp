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

char ch;
char str[400000+100];
int  p[400000+100];
char temp[400000+100];
int mx,id,flag;
int radius;
int center;

int init(char a[])
{
    int ans=1;
    int len=strlen(a);
    for(int i=len;i>=0;i--)
    {
        a[2*i+1]='#';
        a[2*i+2]=a[i];
        ans+=2;
    }
    a[0]='$';
    a[ans]='\0';
    return ans;
}

void Manachar(char *str)
{
    id=0;mx=0;
    flag=init(str);//处理后的长度　其实可以算
    for(int i=1;i<flag;i++)
    {
        p[i]=mx>i?min(p[2*id-i],mx-i):1;
        while(str[i+p[i]]==str[i-p[i]]) p[i]++;
        if(mx<i+p[i])
        {
            mx=i+p[i];
            id=i;
        }
        if(radius<p[i])
        {
            radius=p[i];
            center=i;
        }
    }
}
int main()
{
    while(~scanf("%c %s",&ch,str))
    {
        getchar();
        memset(p,0,sizeof(p));
        radius=0,center=0;
        mx=0,id=0;
        strcpy(temp,str);
        Manachar(str);
        int x=(center-radius)/2+radius-1;
        int y=(center-radius)/2;
        if(x-y<2)
        {
            printf("No solution!\n");
        }else{
            printf("%d %d\n",y,x-1);
            int ans=ch-'a';
            for(int i=y;i<x;i++)
            {
                printf("%c",temp[i]-ans-'a'>=0?temp[i]-ans:temp[i]+26-ans);
            }
            putchar('\n');
        }
    }
}