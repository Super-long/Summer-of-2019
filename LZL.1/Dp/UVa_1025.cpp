#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
#include<unordered_map>
using namespace std;
#define MAX 0x3f3f3f3f
int m;
int ans;
int n,T;
int book[210];
int M1;
int M_one[260];
int M2;
int M_sec[260];
long long temp;

int main()
{
    ans=1;
    while(1)
    {
        bool train[210][55][2];
        int dp[210][55];
        cin >> n;
        if(n==0) break;
        cin >> T;
        for(int i=1;i<n;i++) cin >> book[i];
        cin >> M1;
        while(M1--)
        {
            int tmp;
            cin >> tmp;
			for (int j = 1; j < n; j++) {
				if (tmp <= T)train[tmp][j][0] = 1;
				tmp += book[j];
			}
        }
        cin >> M2;
        while(M2--)
        {
            int tmp;
            cin >> tmp;
			for (int j = n-1; j>0; j--) {
				if (tmp <= T)train[tmp][j+1][1] = 1;
				tmp += book[j];
			}    
        }
        for(int i=1;i<n;i++) dp[T][i]=MAX;
        dp[T][n]=0;//结束标志
        for(int i=T-1;i>=0;i--)
        {
            for(int j=1;j<=n;j++)
            {
                dp[i][j]=dp[i+1][j]+1; //等待时间
                if(j+1<=n && train[i][j][0] && i+book[j]<=T)
                dp[i][j]=min(dp[i][j],dp[i+book[j]][j+1]);
                if(j-1>=1 && train[i][j][1] && i+book[j-1]<=T)
                dp[i][j]=min(dp[i][j],dp[i+book[j-1]][j-1]);
            }
        }
        if(dp[0][1]>=MAX) printf("Case Number %d: %d\n",ans++,dp[0][1]);
        else printf("Case Number %d: impossible\n",ans++);
    }
    return 0;
}