#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
using namespace std;
int x,y,p,q;
long long tmp,tmpa,tmpb,temp;
int main()
{
    cin >> tmp;
    while(tmp--)
    {
        cin >> x >> y >> p >> q;
        if(p==0)
        {
            if(x==0) cout << "0\n";
            else cout << "-1\n";
            continue;
        }
        if(p==q)
        {
            if(x==y) cout << "0\n";
            else cout << "-1\n";
            continue;
        }
        tmpa=((y-x)/(q-p)+(((y-x)%(q-p))?1:0));
        tmpb=((x/p)+((x%p)?1:0));
        temp=max(tmpa,tmpb);
        cout << temp*q-y << endl;
    }
    return 0;
}