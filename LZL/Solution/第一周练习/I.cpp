#include<iostream>
#include<map>
#include<string>
#include<algorithm>
#include<cstdio>
#include<queue>
#include<cstring>
#include<vector>
using namespace std;

int n,l,m;
int bookl[20000+10];
int bookr[20000+10];
int tmp;
char book[100];
int main()
{
    cin >> n;
    while(n--)
    {
        memset(bookl,0,sizeof(bookl));
        memset(bookr,0,sizeof(bookr));
        cin >> l >> m;
        int ll=0,rr=0;
        while(m--)
        {
            cin >> tmp >> book;
            if(book[0]=='l')
            bookl[ll++]=tmp;
            else
            bookr[rr++]=tmp;
        }
        int temp=l*100;
        int flag=1;
        int ans=0;
        int lll=0,rrr=0;
        while(1)
        {
            if(lll>=ll && rrr>=rr) break;
            if(flag)
            {
                int sum=0;
                while((sum+bookl[lll])<=temp && lll<=ll)
                sum+=bookl[lll++];
            }else
            {
                int sum=0;
                while((sum+bookr[rrr])<=temp && rrr<=rr)
                sum+=bookr[rrr++];
            }
            ans++;
            flag=!flag;
        }
        cout << ans << endl;
    }
    return 0;
}