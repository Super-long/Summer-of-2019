//树的层序遍历
#include<iostream>
#include<algorithm>
#include<string>
#include<map>
#include<queue>
#include<vector>
#include<cstdlib>
#include<cstring>
#include<cstdio>
using namespace std;
char s[1000];
bool fail;

struct Node
{
    bool have_value;
    int V;
    Node *lift,*right;
    Node():have_value(false),V(0),lift(nullptr),right(nullptr){}
    //定义了默认构造函数
};
Node *root;

Node* newnode()
{
    return new Node();  //还要多多学习 很高效的一种创建节点的方法
}

void addnode(int v,char * s)
{
    int len=strlen(s);
    Node * tmp=root;
    for(int i=0;i<len;i++)
    {
        if(s[i]=='L')
        {
            if(tmp->lift==nullptr)
            tmp->lift=newnode();
            tmp=tmp->lift;
        }else if(s[i]=='R')
        {
            if(tmp->right==nullptr)
            tmp->right=newnode();
            tmp=tmp->right;
        }
    }
    if(tmp->have_value) fail=true;  
    tmp->have_value=true;
    tmp->V=v;
}

void root_Delete(Node *root)
{
    if(root==NULL) return;
    root_Delete(root->lift);
    root_Delete(root->right);
    delete root;   //new 与 delete 太久没有用过了 应该学习了
}

bool read_input()
{
    fail=false;
    root_Delete(root);
    root=newnode();
    for(;;)
    {   
        getchar();
        if(scanf("%s",s)!=1) return false;   //EOF scanf返回-1
        if(!strcmp("()",s)) break;
        int v;
        sscanf(&s[1],"%d",&v);
        addnode(v,strchr(s,',')+1);    //这两个函数是精髓
    }
    return true;
}

bool bfs(vector<int> & vec)  //从来没把vector做过参数 降低程序之间耦合性 很好
{
    queue<Node*>que;
    que.push(root);
    Node *temp;
    while(que.size())
    {
        temp=que.front();
        que.pop();
        if(!temp->V) return false;  //有结点上没有值 
        vec.emplace_back(temp->V);
        if(temp->lift!=nullptr)
        que.push(temp->lift);
        if(temp->right!=nullptr)
        que.push(temp->right);
    }
    return true;
}

int main()
{
    while(1)
    {
        if(!read_input()) break;
        if(fail) 
        {
            fail=false;
            cout << "not complete\n";
            continue;
        }
        vector<int>vec;
        if(!bfs(vec))
        {
            cout << "not complete\n";
            continue;
        }
        else 
        for(auto x=vec.begin();x!=vec.end();x++)
        {
            if(x==vec.begin()) cout << *x;
            else cout << " " << *x;
        }
        putchar('\n');
    }
    return 0;
}