#include<iostream>
#include<algorithm>
#include<string>
#include<map>
#include<set>
#include<unordered_map>
#include<list>
#include<vector>
#include<cstdlib>
#include<utility>
#include<cstring>
using namespace std;
int m,n;
int tmpa,tmpb;
struct node
{
    int first;
    int second;
};
node book[300000+10];

int judge(int seq,int tmp,int start)
{
    for(int i=start;i<n;i++)
    {
        if((book[i].first==seq || book[i].second==tmp) || (book[i].first==tmp || book[i].second==seq))
        ;else {
            return 0;
        }
    }
    return 1;
}

int main()
{
    scanf("%d %d",&m,&n);
    for(int i=0;i<n;i++){
        scanf("%d %d",&book[i].first,&book[i].second);
    }
    for(int i=1;i<n;i++)
    {
        if(book[i].first!=book[0].first && book[i].second!=book[0].second && book[i].first!=book[0].second && book[i].second!=book[0].first)
        {
            if(judge(book[0].first,book[i].first,i)+judge(book[0].first,book[i].second,i)>=1)
            {
                printf("YES\n");
                return 0;
            }else if(judge(book[0].second,book[i].first,i)+judge(book[0].second,book[i].second,i)>=1)
            {
                printf("YES\n");
                return 0;
            }else{
                printf("NO\n");
                return 0;
            }
        }
    }
    printf("YES\n"); //第一项的两个数就是答案
    return 0;
}