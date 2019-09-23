//栈模拟
#include<iostream>
#include<algorithm>
#include<vector>
#include<stack>
using namespace std;

int m;
int book[1000+10];

int main()
{
    while(cin >> m && m!=0)
    {
        book[0]=1;
        while(book[0]!=0)
        {
            stack<int>stk;
            for(int i=0;i<m;i++)
            {
                cin >> book[i];
                if(i==0 && book[i]==0) break;
                stk.push(book[i]);
            }
            int flag=m;
            stack<int>sta;
            if(stk.size())
            {
                sta.push(stk.top());
                stk.pop();
            }else break; //防止为空的情况
            while(sta.size())
            {
                int in=0;
                if(sta.size() && sta.top()==flag)
                {
                    flag--;
                    sta.pop();
                    in=1;
                }
                if(stk.size())
                {
                    sta.push(stk.top());
                    stk.pop();
                    in=1;
                }
                if(!in) break;
            }
            if(sta.size()==0) cout << "Yes\n";
            else cout << "No\n";
        }
        putchar('\n');
    }
}