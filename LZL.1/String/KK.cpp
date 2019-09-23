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

//指针法　也可以写
typedef struct node
{
    int ans;
    struct node *ptr[27];
    bool type; //标志是否这个点有值
    void tree()
    {
        ans=0;
        type=false;
        memset(ptr,-1,sizeof(ptr));
    }
}tree_t;
tree_t book[100000+10];
int ans;

int insert(char tmp[])
{
   int len=strlen(tmp);
   for(int i=0;i<len;i++)
   {
       int r=tmp[i]-'a';
       if()
   } 
}
char temp[12];
int main()
{
    book[0].tree();
    while(cin >> temp)
    {
        insert(temp);
    }
}