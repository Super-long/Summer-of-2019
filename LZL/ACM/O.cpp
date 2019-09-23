#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
#include<unordered_map>
using namespace std;
long long k,a,b;
long long tmpa,tmpb;
int main()
{
    cin >> k >> a >> b;
    tmpa=max(a,b);
    tmpb=min(a,b);
    if(tmpa < k) cout << "-1\n";
    else if(tmpa%k && tmpb<k) cout << "-1\n";
    else cout << tmpa/k+tmpb/k << endl;
    return 0;
}