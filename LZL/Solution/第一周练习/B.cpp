#include<iostream>
#include<algorithm>
#include<string>
#include<map>
#include<vector>
#include<cstring>
using namespace std;
int m,n;
int book[1000+10];
map<int,int>mp;
int ans=0;
int main()
{
    cin >> m;
    while(m--)
    {
        ans=0;
        mp.clear();
        cin >> n;
        for(int i=0;i<n;i++)
        {
            cin >> book[i];
            if(book[i]%3==0)
            {
                ans++;
            }else if(book[i]%3==1)
            {
                mp[1]++;
            }else if(book[i]%3==2)
            {
                mp[2]++;
            }
        }
        if(mp[2]>=mp[1])
        {
            cout << mp[2]+ans << endl;
        }else if(mp[2]<mp[1])
        {
            cout << mp[2]+ans+(mp[1]-mp[2])/3 << endl;
        }
    }
    return 0;
}