#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
#include<unordered_map>
#include<cstring>
using namespace std;

int p[250000];

char str[250000];
int flag;
int mx,id;
int radius;
int center;


int init(char a[])
{
    int ans=1;
	int len = strlen(a);
	for (int i=len;i>=0;i--)
	{
		a[2*i+1]='#';
		a[2*i+2]=a[i];
        ans+=2;
	}
	a[0]='$';
    a[ans]='\0';
    return ans;
}

int judge(char *tmp)
{
    id=0;mx=0;
    //char tmp[250000]="$#";
    flag=init(tmp);
    for(int i=1;i<flag;i++)
    {
        p[i] = mx>i?min(p[2*id-i],mx-i):1;
        while(tmp[i+p[i]]==tmp[i-p[i]]) p[i]++;
        if(mx<i+p[i])
        {
            mx=i+p[i];
            id=i;
        }
        if(radius<p[i])
        {
            radius=p[i];
            center=i;
        }
    }
    return radius-1;
}

int main()
{
    while(~scanf("%s",str))
    {
        center=0;
        radius=0;
        cout << judge(str) << endl;
        memset(p,0,sizeof(p));
    }
    return 0;
}