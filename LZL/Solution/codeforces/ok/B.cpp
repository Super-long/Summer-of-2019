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
map<int,int>mp;
int n,k;
int main()
{
    cin >> n >> k;
    for(int i=0;i<=1000000000;i++)
    {
        if((n-i)*(n-i+1)-2*k==2*i)
        {
            cout << i << endl;
            break;
        }
    }
    return 0;
}