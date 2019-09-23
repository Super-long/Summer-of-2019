#include<iostream>  
#include<stdio.h>  
#include<string.h>  
#include<algorithm>  
using namespace std;  
int book[1000+10][1000+10];  
int tmpa,tmpb,m,n,ans;
int main()  
{  
    int q;
    scanf("%d",&q);  
    while(q--)  
    {  
        scanf("%d %d %d %d",&tmpa,&tmpb,&m,&n);  
        memset(book,0,sizeof(book));  
        for(int i=1;i<=tmpa;i++)  
            for(int j=1;j<=tmpb;j++)  
            {  
                int tmp;
                scanf("%d",&tmp);  
                book[i][j]=book[i-1][j]+book[i][j-1]+tmp-book[i-1][j-1];  
            }  
        ans=0;  
        for(int i=m;i<=tmpa;i++)  
        for(int j=n;j<=tmpb;j++)  
        ans=max(ans,book[i][j]-book[i-m][j]-book[i][j-n]+book[i-m][j-n]);  
        printf("%d\n",ans);  
    }  
    return 0;  
} 