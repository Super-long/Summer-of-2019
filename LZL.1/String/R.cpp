#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<cstdio>
#include<cstring>
#include<cstring>
using namespace std;

vector<string>vec;
string str;
int tree[200000][30];
int num[200000];
int tot=1,root=1;

void insert(string & s){
    int root = 1;
    for(int i = 0; i < s.length(); i ++){
        int a = s[i] - 'a';
        if(tree[root][a] == 0) 
        tree[root][a] = ++tot;
        root=tree[root][a];
        num[root] ++;
    }
}
int query(string & str){
    int root=1;
    cout << str <<" ";
    for(int i = 0; i < str.length(); i ++){
        if(num[root] == 1)
        return 0;
        int a=str[i] - 'a';
        cout<<str[i];
        root=tree[root][a];
    }
}
int main()
{
    while(cin >> str)
    {
        vec.push_back(str);
        insert(str);
    }
    int x=vec.size();
    for(int i=0;i<x;i++)
    {
        query(vec[i]);
        putchar('\n');
    }
    return 0;
}