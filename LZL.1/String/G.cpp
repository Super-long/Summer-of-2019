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

char book[100000+10];
int ans;

int judge(char *str)
{
    //printf("\n::%s",str);
    char *tmp;
    char *tt=str;
    int flag=0;
    while((tmp=strstr(str,"doge"))!=NULL)
    {
/*         printf("::%s\n",str);
        printf("%s\n",tmp); */
        flag++;
        str=tmp+4;
    }
    return flag;
}
int main()
{
    char ch;
    int dd=0;
    while(1)
    {   
        int flag=0;
        while(ch=getchar())
        {
            if(ch=='\n') break;
            if(ch==EOF)
            {
                dd=1;
                break;
            }
            if(ch>=65 && ch<=90)
            ch+=32;
            book[flag++]=ch;
        }
        if(dd) break;
        book[flag]='\0';
        ans+=(judge(book));
    }
    printf("%d\n",ans);
}