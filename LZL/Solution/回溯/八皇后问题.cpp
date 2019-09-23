#include<iostream>
#include<algorithm>
#include<string>
using namespace std;
int book[10];
int n=4;
int tot;
int judge(int cur)
{
    if(cur==n)
    tot++;
    else
    {
        for(int i=0;i<n;i++)
        {
            int ok=1;
            book[cur]=i;
            for(int j=0;j<cur;j++)
            {
                if(book[cur]==book[j] || book[cur]-cur==book[j]-j || book[cur]+cur==book[j]+j)
                {
                    ok=0;break;
                }
            }
            if(ok) judge(cur+1);
        }
    }
}
int main()
{
    judge(0);
    cout << tot << endl;
    return 0;
}