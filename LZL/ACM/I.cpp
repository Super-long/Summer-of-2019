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
int book[200000+10];
int main()
{
    cin >> m;
    for(int i=1;i<=m;i++)
    {
        cin >> book[i];
    }
    for(int i=1;i<=m/2;i+=2)
    {
        swap(book[i],book[m-i+1]);
    }
    for(int i=1;i<=m;i++)
    if(i==1) cout << book[i];
    else cout << " " << book[i];
    putchar('\n');
    return 0;
}