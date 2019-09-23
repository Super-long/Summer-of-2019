/* //模拟
#include<iostream>
#include<algorithm>
#include<string>
#include<stack>
#include<queue>
using namespace std;
int m,n;
int book[200000+10];
typedef struct 
{
    int index;
    int price;
}node;

int main()
{   
    cin >> m;
    while(m--)
    {
        cin >> n;
        int sum=0;
        int flag=0;
        for(int i=1;i<=n;i++)
        {
            cin >> book[i];
            sum+=book[i];
            if(sum>=n)
            {
                sum-=n;
            }
        }
        if(sum==0)
        {
            cout << "YES\n";
        }else
        {
            cout << "NO\n";
            continue;
        }
        stack<node>up;
        stack<node>down;
        node temp;
        int ans=0;//记录操作数
        for(int i=1;i<=n;i++)
        {
            if(book[i]>flag)
            {
                temp.index=i;
                temp.price=book[i];
                up.push(temp);
                ans+=(book[i]-flag);
            }
            if(book[i]<flag)
            {
                temp.index=i;
                temp.price=book[i];
                down.push(temp);
            }
        }
        cout << ans << endl;  //操作数
        while(!up.empty())
        {
            node tmpa=up.top();
            up.pop();
            node tmpb=down.top();
            down.pop();
            while(1)
            {
                cout << tmpa.index-- << " " << tmpb.index++;
                if(tmpa.price==0 || tmpb.price==6) break;
            }
            if(tmpa.price-6>0) up.push(tmpa);
            if(tmpb.price-6<0) down.push(tmpb);
        }
    }
    return 0;
} */

#include<iostream>
#include<algorithm>
#include<string>
#include<stack>
#include<queue>
using namespace std;
int m,n;
int book[200000+10];
typedef struct 
{
    int index;
    int price;
}node;
int main()
{
    cin >> m;
    while(m--)
    {
        cin >> n;
        int sum=0;
        int flag=0;
        for(int i=1;i<=n;i++)
        {
            cin >> book[i];
            sum+=book[i];
            if(sum>=n)
            {
                flag+=(sum/n);//flag为平均值
                sum%=n;
            }
        }
        if(sum==0)
        {
            cout << "YES\n";
        }else
        {
            cout << "NO\n";
            continue;
        }
        int i=1;
        queue<node>que;
        int ans=0;
        while(i<=n)
        {
            if(book[i]<flag)
            {
                node tmp;
                tmp.index=i+1;  //a
                tmp.price=i;    //b   a给b
                book[i+1]--;
                book[i]++;
                que.push(tmp);
                ans++;
            }else if(book[i]>flag)
            {
                node tmp;
                tmp.index=i;
                tmp.price=i+1;
                book[i]--;
                book[i+1]++;   
                que.push(tmp);
                ans++;             
            }else
            {
                i++;//这一项达到平衡
            }
        }
        cout << ans << endl;
        while(que.size())
        {
            node temp=que.front();
            que.pop();
            cout << temp.index << " " << temp.price << endl;
        }
    }
    return 0;
}