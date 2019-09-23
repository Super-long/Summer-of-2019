#include<iostream>
#include<map>
#include<algorithm>
#include<cstdio>
#include<cstring>
using namespace std;
int m,n,k;
char book[200000+10];
int judge(int ans)
{
    int kk=0;
    int i=0;
    for(i=0;i<n;)
    {
        if(kk>k) break;
        if(book[i]=='0') i++;
        else
        {
            kk++;
            i+=ans;
        }
    }
    if(i>=n && kk<=k) return 1;
    else return 0;
}

int main()
{
    cin >> m;
    while(m--)
    {
        memset(book,0,sizeof(book));
        cin >> n >> k;
        cin >> book;
        int l=1,r=200000+10;
        while(l<=r)
        {
            int mid=(l+r)/2;
            if(judge(mid))
            {
                r=mid-1;
            }else
            {
                l=mid+1;
            }  
        }
        cout << l << endl;
    }
}