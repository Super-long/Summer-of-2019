#include<iostream>
#include<algorithm>
#include<string>
#include<map>
#include<vector>
#include<cstring>
using namespace std;
int m;
int book[200000+10];
int main()
{
    cin >> m;
    for(int i=1;i<=m;i++)
    {
        cin >> book[i];
    }
    sort(book+1,book+1+m);
    int ans=1;
    int i=1;
    int flag=1;
    while(1)
    {
        if(i>m) break;
        if(book[i++]>=ans)
        ans++;
    }
    cout << ans-1 << endl;
    return 0;
}