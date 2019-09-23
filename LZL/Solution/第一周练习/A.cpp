#include<iostream>
#include<algorithm>
#include<string>
#include<map>
#include<vector>
#include<cstring>
using namespace std;
int m;
long long int n;

int judge(long long tmp,long long  ans)
{
    if(tmp%2==0)
    {
        return judge(tmp/2,ans+1);
    }else if(tmp%3==0)
    {
        return judge(tmp/3*2,ans+1);
    }else if(tmp%5==0)
    {
        return judge(tmp/5*4,ans+1);
    }else if(tmp==1)
    {   
        return ans;
    }   
    return -1;
}
int main()
{
    cin >> m;
    while(m--)
    {
        cin >> n;
        cout << (n==1?0:judge(n,0)) << endl;
    }
}