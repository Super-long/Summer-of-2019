#include<iostream>
#include<map>
#include<string>
#include<algorithm>
#include<cstdio>
#include<queue>
#include<cstring>
#include<vector>
using namespace std;
int m;
char ch;
string book;
string tmp;
queue<char>que;
vector<char>vec;
int main()
{
    cin >> m;
    cin >> book;
    for(int i=0;i<book.size();i++)
    if(!(tmp.size()%2) || book[i]!=tmp.back())
    tmp+=book[i];
    if(tmp.size()%2) tmp.pop_back();
    cout << book.size()-tmp.size() << endl;
    cout << tmp << endl;
    return 0;
}