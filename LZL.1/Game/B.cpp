#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
#include<unordered_map>
using namespace std;
int m,n;
int judge(int a,int b)
{
    if(a==b || a>2*b)
    cout << "Stan wins\n";
    else if(a>b && a<2*b)
    return 0;
    else if()
}
int main()
{
    while(cin >> m >> n)
    {
        if(m==0 && n==0) break;
        int a=max(m,n);
        int b=min(m,n);
        while(judge(a,b));
    }   
}