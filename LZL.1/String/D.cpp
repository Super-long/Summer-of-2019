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
char book[100000+10];//母串
int  Next[100000+10];
int q,flag,m;
void kmp()
{
    int n=strlen(book);
    int i=0,j=-1;
    Next[0]=-1;
    while(i<n)
    {
        if(j==-1 || book[i]==book[j])
        {
            i++;j++;
            Next[i]=j;
        }else{
            j=Next[j];
        }
    }
}

int main()
{
    while(cin >> m && m)
    {
        memset(book,0,sizeof(book));
        cin >> book;
        kmp();
        cout << "Test case #" << ++q << endl;
        int x=strlen(book);
        int tt=0;
        for(int i = 2 ; i <= x ; i ++)
		{
			int j = i - Next[i];
			if(i%j==0 && i/j >1) cout << i << " " << i/j << endl; 
		}
        putchar('\n');
    }
    return 0;
}