#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
using namespace std;
int m;
int book[31];
int main()
{
    cin >> m;
    for(int i=0;i<m;i++)
    {
        cin >> book[i];
    }
    sort(book,book+m);
    int tmp=book[m-1]-book[0];
    cout << (book[0]+((tmp%2)?(tmp/2+1):(tmp/2))) << endl;
    return 0;
}