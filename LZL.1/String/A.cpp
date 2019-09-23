#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
#include<unordered_map>
using namespace std;
int m,n;
int book[1000000+10];
int res[1000000+10];

void kmp(int *next){     //next第一项为零
	next[0]=0;
	int j=0,i=1;
	while(i<m){
		if(res[j]==res[i]){
			next[i]=++j;
			i++;
		}
		else if(j==0){
			i++;
		}
		else{
			j=next[j-1];//从失配位置开始匹配
		}
	}
}

int solve(int *next)
{
    int i=0;
    int j=0;
    int ans=0;
    while(i<m && j<n)
    {    //母串       子串
        if(book[i]==res[j])
        {
            i++;
            j++;
        }else if(j==0){
            i++;
        }else{
            j=next[j-1];
        }
    }
    if(j==n) return ans = i-n+1;
    else return ans = -1;
}

int main()
{
    int q=0;
    scanf("%d",&q);    //以后所有题目用scanf
    int next[1000000+10];
    while(q--)
    {
        memset(next,0,sizeof(next));
        scanf("%d %d",&m,&n);
        for(int i=0;i<m;i++){
            scanf("%d",&book[i]);
        }
        for(int i=0;i<n;i++){
            scanf("%d",&res[i]);
        }
        kmp(next);
        printf("%d\n",solve(next));
    }
    return 0;
}