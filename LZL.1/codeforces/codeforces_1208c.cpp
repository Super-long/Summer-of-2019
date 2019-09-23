#include<bits/stdc++.h>
using namespace std;
 
int n,cnt;
int book[1010][1010];
 
inline void prework()
{
}
 
inline void solve(int x,int y)
{
	for(int i=0;i<4;i++)	
		for(int j=0;j<4;j++)
			book[x+i][y+j]=cnt++;
}
 
inline void mainwork()
{
	for(int i=1;i<=n;i+=4)
		for(int j=1;j<=n;j+=4)
			solve(i,j);
}
 
int main()
{
	cin >> n;
	cnt=0;
	prework();
	mainwork();
	for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=n;j++)
        {
            if(j==1) cout << book[i][j];
            else cout << " " << book[i][j];
        }
        putchar('\n');
    }
	return 0;
}