#include<iostream>
#include<algorithm>
#include<map>
#include<cstring>
#include<unordered_map>
using namespace std;

int q,m,tmp,flag;
int main()
{
    cin >> q;
    while(q--)
    {
        int ans=0;
        cin >> m;
        cin >> ans;
        m--;
        ans=max(0,ans-2);
        while(m--)
        {
            cin >> tmp;
            tmp=max(0,tmp-2);
            ans^=tmp;
        }
        cout << "Case " << ++flag << ": " << ans << endl;
    }
    return 0;
}