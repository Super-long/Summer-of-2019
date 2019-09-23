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
#include<cmath>
using namespace std;
int m,n;
int book[300];
int flag;
int main()
{
    cin >> m;
    while(m--)
    {
        memset(book,0,sizeof(book));
        flag=0;
        cin >> n;
        for(int i=0;i<n;i++)
        {
            cin >> book[i];
        }
        for(int i=1;i<n;i++)
        {
            if(abs(book[i]-book[i-1])!=1 && abs(book[i]-book[i-1]!=n-1))
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