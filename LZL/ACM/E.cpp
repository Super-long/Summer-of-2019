#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
using namespace std;

int m,n;
int tmp=1;
int main()
{
    cin >> m >> n;
    for(int i=1;i<=m;i++)
    {
        if(n)
        {
            cout << char(i+'a'-1);
            n--;
        }
        else
        {
            if(tmp==1)
            cout << "a";
            else 
            cout << "b";
            tmp=!tmp;
        }
    }
    putchar('\n');
    return 0;
}