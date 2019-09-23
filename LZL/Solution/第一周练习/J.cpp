#include<iostream>
#include<map>
#include<string>
#include<algorithm>
#include<cstdio>
#include<queue>
#include<cstring>
#include<vector>
using namespace std;
int m;
int book[200000+10];
queue<char>que;
int main()
{
    cin >> m;
    for(int i=1;i<=m;i++)
    {
        cin >> book[i];
    }
    int l=1,r=m;
    int tmp=0;
    if(book[l]>book[m])
    {
        que.push('R');
        tmp=book[r--];
    }else 
    {
        que.push('L');
        tmp=book[l++];
    }
    int sum=1;
    while(1)
    {
        if(book[l]<=tmp && book[r]<=tmp) break;
        if(l==r)
        {
            que.push('L');
            sum++;
            break;
        }
        if(book[l]<book[r])
        {
            if(book[l]>=tmp)
            {
                que.push('L');
                sum++;
                tmp=book[l++];
            }else
            {
                que.push('R');
                sum++;
                tmp=book[r--];
            }    
        }else
        {   
            if(book[r]>=tmp)
            {
                que.push('R');
                sum++;
                tmp=book[r--];
            }else
            {
                que.push('L');
                sum++;
                tmp=book[l++];
            }    
        }
    }
    cout << sum << endl;
    while(que.size())
    {
        cout << que.front();
        que.pop();
    }
    putchar('\n');
    return 0;
}