#include<iostream>
#include<algorithm>
#include<string>
#include<map>
#include<vector>
#include<cstdlib>
#include<cstring>
using namespace std;

int m;

bool solve(int &w)    //改变本层元素的条件决定与下一层递归 引用是个很优秀的选择
{
    int w1,d1,w2,d2;     //有单个重物的情况
    bool b1=true,b2=true;
    cin >> w1 >> d1 >> w2 >> d2;
    if(!w1) b1=solve(w1);
    if(!w2) b2=solve(w2);   //巧妙
    w=w1+w2;
    return b1 && b2 && (w1*d1 == w2*d2);
}

int main()
{
    int x=0;
    cin >> m;
    while(m--)
    {
        if(solve(x)) cout << "YES\n";
        else cout << "NO\n";
        if(m) cout << endl;
    }
    return 0;
}