#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
using namespace std;

vector<int>vec[110];
vector<int>temp;
int in[110];
int m,n;
queue<int>que;
int tmpa,tmpb;

int topo()
{
    while(que.size())
    {
        int tmp=que.front();
        temp.push_back(tmp);
        que.pop();
        for(auto x : vec[tmp])
        {
            if((--in[x])==0)
            que.push(x);
        }
    }
    if(temp.size()==tmpa)  //无环
    return 1;
    return 0;
}
int main()
{
    while(cin >> tmpa >> tmpb)
    {
        temp.clear();
        for(int i=0;i<110;i++)vec[i].clear();
        if(tmpa==0 && tmpb==0) break;
        for(int i=0;i<tmpb;i++)
        {
            cin >> m >> n;
            vec[m].push_back(n);
            in[n]++;
        }
        for(int i=1;i<=tmpa;i++)
        {
            if(in[i]==0) que.push(i);
        }
        if(topo())
        for(int i=0;i<temp.size();i++)
        {
            if(!i)cout << temp[i];
            else cout << " " << temp[i];
        }
        putchar('\n');
    }
}