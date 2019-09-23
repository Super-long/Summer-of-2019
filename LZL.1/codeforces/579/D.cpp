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
char tmpa[250];
char tmpb[250];
int flag,i;
int main()
{
    cin >> tmpa >> tmpb;
    int a=strlen(tmpa);
    int b=strlen(tmpb);
    int tt=b-1;
    int ans=0;
    int dd=0;
    for(i=0;i<a;i++)
    {
        dd++;
        if(tmpa[i]==tmpb[ans])
        {
            dd=0;
            ans++;
        }
        flag=max(dd,flag);
        if(ans==b) break;
    }
    flag=a-i-1;
    dd=0;
    for(i=a-1;i>=0;i--)
    {
        dd++;
        if(tmpa[i]==tmpb[tt])
        {
            tt--;
            dd=0;
            ans++;
        }
        flag=max(dd,flag);
        if(tt<0) break;
    }
    flag=max(flag,i);
    cout << flag << endl;
    return 0;
}