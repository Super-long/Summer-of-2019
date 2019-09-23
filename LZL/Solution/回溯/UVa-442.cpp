//简单的模拟 中间少了一个++ 看了好久 有点伤 
#include<iostream>
#include<algorithm>
#include<string>
#include<map>
#include<stack>
#include<vector>
#include<cstring>
using namespace std;

int m,tmpa,tmpb,flag;
char ch;

typedef struct 
{
    int first,second;
}node;
node book[100000+10];
int ans=0;

int main()
{
    while(cin >> m)
    {
        for(int i=0;i<m;i++)
        {
            cin >> ch >> tmpa >> tmpb;
            book[ch-'A'].first =tmpa;
            book[ch-'A'].second=tmpb;
        }
        int vis=m;
        getchar();
        int run=1;
        while(1)
        {
            flag=0;
            ans=0;
            stack<char>stk;
            vis=m;
            run=1;
        while(ch=getchar())
            {
                if(ch=='\n') break;
                if(ch==EOF) {run=0;break;}
                if(ch=='(')
                {
                    stk.push(ch);
                }else if(ch==')')
                {
                    tmpa=stk.top();
                    stk.pop();
                    tmpb=stk.top();
                    stk.pop();
                    if(book[tmpa].first==book[tmpb].second)
                    {
                        ans+=book[tmpa].first*book[tmpa].second*book[tmpb].first;
                        stk.pop();
                        node tmep={book[tmpb].first,book[tmpa].second};
                        book[vis]=tmep;
                        stk.push(vis);
                        vis++;
                    }else
                    {
                        cout << "error\n";
                        flag=1;
                        getchar();
                        break;
                    }
                }else stk.push(ch-'A');
            }
            if(run==0)break;
            if(flag==0)
            {
                cout << ans << endl;
                flag=0;
            }
        }
    }
    return 0;
}