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
int q;
int book[500];
int flag;
int main()
{
    cin >> q;
    while(q--)
    {
        memset(book,0,sizeof(book));
        flag=0;
        cin >> m;
        for(int i=1;i<=4*m;i++)
        cin >> book[i];
        sort(book+1,book+1+4*m);
        int ttt=0;
        for(int i=1;i<=4*m;i+=2)
        {
            if(book[i]!=book[i+1])
            {
                cout << "NO\n";
                ttt=1;
                break;
            }
        }
        if(ttt)continue;
        int tmp=4*m;
        int ans=book[1]*book[tmp];
        for(int i=1;i<=2*m;i+=2)
        {
            if(book[i]*book[tmp-i]!=ans)
            {
                flag=1;
                break;
            }
        }
        if(flag) cout << "NO\n";
        else cout << "YES\n";
    }
    return 0;
}