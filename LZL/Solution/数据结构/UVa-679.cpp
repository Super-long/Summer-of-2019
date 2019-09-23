#include<iostream>
#include<algorithm>
#include<string>
#include<map>
#include<vector>
#include<cstdlib>
#include<cstring>
using namespace std;

int m,tmpa,tmpb;
int main()
{
    while(cin >> m)
    {
        if(m==-1)break;
        for(int i=0;i<m;i++)
        {
            cin >> tmpa >> tmpb;   //深度与小球个数
            int flag=1;
            for(int j=0;j<tmpa-1;j++)
            {
                if(!(tmpb%2))
                {
                    flag=flag*2+1;
                    tmpb/=2;
                }else
                {
                    flag=flag*2;
                    tmpb=(tmpb+1)/2;
                }
            }
            cout << flag << endl;
        }
    }
    return 0;
}

/* int main()
{
    while(cin >> m)
    {
        if(m==-1)break;
        for(int i=0;i<m;i++)
        {
            cin >> tmpa >> tmpb;   //深度与小球个数
            memset(book,0,sizeof(book));
            int flag=1;
            while(tmpb--)
            {
                flag=1;
                while(1)
                {
                    book[flag]=!book[flag];
                    flag=book[flag]?(flag*2):flag*2+1;
                    if(flag>((1<<tmpa)-1)) break;         //很漂亮的代码 可惜超时了
                }
            }
            cout << flag/2 << endl;
        }
    }
    return 0;
} */