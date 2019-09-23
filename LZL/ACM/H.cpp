#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
#include<unordered_map>
using namespace std;
int m;
int book[100000+10];
unordered_map<int,int>mp;
int main()
{
    cin >> m;
    for(int i=0;i<m;i++)
    {
        cin >> book[i];
        mp[book[i]]++;
    }
    sort(book,book+m);
    cout << max(m-mp[book[0]]-mp[book[m-1]],0) << endl;
    return 0;
}