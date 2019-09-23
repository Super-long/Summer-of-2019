#include<iostream>
#include<sstream>

using namespace std;
#define maxn 1010

int n;
int ltree[maxn],rtree[maxn],in_order[maxn],post_order[maxn];

bool read_list(int *a)//数组指针的好处在于实现代码重用 因为这个指针可以代表任意数组
{  
    string str;
    if(!getline(cin,str)) return false;
    stringstream ss(str);      //十分灵活的方法
    int x=0;
    n=0;
    while(ss >> x)
    a[n++]=x;          
    return n>0;
}


//方法记住 这类题就这种方法
int build(int L1,int R1,int L2,int R2)
{
    if(L1>R1) return 0;  //到达叶子节点
    int root=post_order[R2]; //根节点
    int net=L1;           //第一遍错误 写成了0
    while(in_order[net]!=root) net++;
    int cnt=net-L1; //左子树数量
    ltree[root]=build(L1,net-1,L2,L2+cnt-1);
    rtree[root]=build(net+1,R1,L2+cnt,R2-1);
    return root;
}

int best,best_sum;

//注意递归中值会改变的参数 用脑子模拟一两遍
int dfs(int u,int sum)  //传入sum要带根节点的值
{
    sum+=u;
    if(!ltree[u] && !rtree[u])//此节点为叶子节点
    {
        if(sum<best_sum || (sum==best_sum && u<best))
        {
            best = u;
            best_sum=sum;
        }
    }
    if(ltree[u]) dfs(ltree[u],sum);
    if(rtree[u]) dfs(rtree[u],sum); //会损失掉叶子节点的值
}

int main()
{
    while(read_list(in_order))
    {
        read_list(post_order);
        build(0,n-1,0,n-1);
        best=0;best_sum=100000;
        dfs(post_order[n-1],0);
        cout << best << endl;
    }
    return 0;
}