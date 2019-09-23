#include<iostream>
#include<algorithm>
using namespace std;

int m,n;
int book[2][100000+10];
int64_t dp[2][100000+10]; //unsiged long 

int main()
{
    //cin >> m;
    int i=0;
    for(cin >> m;i<m+m;i++) cin >> book[i/m][i%m+1];
/*     for(int i=0;i<2;i++)
    {
        for(int j=1;j<=m;j++)
        {
            cin >> book[i][j];
        }
    } */
    for(int i=1;i<=m;i++)
    {
        for(int j=0;j<2;j++)
        {
            dp[j][i]=max(dp[!j][i-1]+book[j][i],dp[j][i-1]);
        }
    }
    cout << max(dp[1][m],dp[0][m]) << endl;
    return 0;
}