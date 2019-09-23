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
char book[100000+10];//母串
int q,flag;
void kmp(int *next){
	next[0]=0;
    int m=strlen(book);
	int j=0,i=1;
	while(i<m){
		if(book[j]==book[i]){
			next[i]=++j;
			i++;
		}
		else if(j==0){
			i++;
		}
		else{
			j=next[j-1];
		}
	}
}

int main() //循环字符串next数组连续
{
    scanf("%d",&q);
    int next[100000+10];
    while(q--)
    {
        int dd=0;
        int ans=0;
        memset(next,0,sizeof(book));
        scanf("%s",book);
        kmp(next);
        int x=strlen(book);
        int num=x-next[x-1];//循环结大小
        if(next[x-1]==0){//保证循环
            printf("%d\n",x);
        }else if(x%(num)==0){
            printf("0\n");
        }else{
            printf("%d\n",num-(x%num));
        }
        memset(next,0,sizeof(next));
    }
    return 0;
}